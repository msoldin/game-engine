module;
#include <algorithm>
#include <ranges>
#include <vector>
export module vulkan_engine.ecs:component_lookup_table;

import :component;

namespace vulkan_engine::ecs {
struct ComponentLookupTableEntry {
  size_t entity_id;
  ComponentTypeId component_type_id;
  IComponent* component;

  // Comparison operator for sorting and binary search
  bool operator<(const ComponentLookupTableEntry& other) const {
    return std::tie(entity_id, component_type_id) < std::tie(other.entity_id, other.component_type_id);
  }

  bool operator==(const ComponentLookupTableEntry& other) const { return entity_id == other.entity_id && component_type_id == other.component_type_id; }
};

class ComponentLookupTable {
 public:
  static auto key_comparator(const ComponentLookupTableEntry& a, const ComponentLookupTableEntry& b) { return a < b; }

  static auto entity_id_comparator(const ComponentLookupTableEntry& a, const ComponentLookupTableEntry& b) { return a.entity_id < b.entity_id; }

  template <IsComponent T>
  void addComponent(const size_t entity_id, IComponent* component) {
    // Binary search for position
    // Use lower_bound to find where the component should be inserted
    const ComponentTypeId component_type_id = T::getComponentTypeId();
    const ComponentLookupTableEntry key{.entity_id = entity_id, .component_type_id = component_type_id, .component = component};
    const auto it = std::ranges::lower_bound(m_components, key, key_comparator);
    m_components.insert(it, key);
  }

  template <IsComponent T>
  void removeComponent(const size_t entity_id) {
    const ComponentTypeId component_type_id = T::getComponentTypeId();
    const ComponentLookupTableEntry key{.entity_id = entity_id, .component_type_id = component_type_id, .component = nullptr};
    if (const auto it = std::ranges::lower_bound(m_components, key, key_comparator); it != m_components.end() && *it == key) {
      m_components.erase(it);
    }
  }

  template <IsComponent T>
  std::optional<std::reference_wrapper<T>> getComponent(const size_t entity_id) const {
    const ComponentTypeId component_type_id = T::getComponentTypeId();
    const ComponentLookupTableEntry key{.entity_id = entity_id, .component_type_id = component_type_id, .component = nullptr};
    if (const auto it = std::ranges::lower_bound(m_components, key, key_comparator); it != m_components.end() && *it == key) {
      return std::ref(*dynamic_cast<T*>(it->component));  // Return the IComponent pointer
    }
    return std::nullopt;
  }

  template <IsComponent... Ts>
  auto getComponents(const size_t entity_id) const {
    std::tuple<std::optional<std::reference_wrapper<Ts>>...> result;

    // Binary search for the first occurrence of the entity
    const ComponentLookupTableEntry key{.entity_id = entity_id, .component_type_id = 0, .component = nullptr};
    const auto it = std::ranges::lower_bound(m_components, key, entity_id_comparator);

    // Iterate over all matching entries
    for (auto entry = it; entry != m_components.end() && entry->entity_id == entity_id; ++entry) {
      // Expand the fold expression outside the lambda
      (
          [&]<typename T>() {
            if (!std::get<std::optional<std::reference_wrapper<T>>>(result).has_value() && entry->component_type_id == T::getComponentTypeId()) {
              if (auto* component = dynamic_cast<T*>(entry->component)) {
                std::get<std::optional<std::reference_wrapper<T>>>(result) = std::ref(*component);
              }
            }
          }.template operator()<Ts>(),
          ...);  // Fold expression properly expanded
    }

    return result;
  }

 private:
  std::vector<ComponentLookupTableEntry> m_components;
};

}  // namespace vulkan_engine::ecs