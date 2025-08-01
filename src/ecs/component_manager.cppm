module;
#include <glog/logging.h>

#include <algorithm>
#include <memory>
#include <ranges>
#include <unordered_map>
#include <vector>
export module game_engine.ecs:component_manager;

import :component;
import :component_lookup_table;

import game_engine.memory;
import game_engine.error;

namespace game_engine::ecs {

class IComponentRegistry {
 public:
  virtual ~IComponentRegistry() = default;
};

template <typename T>
class ComponentRegistry final : public IComponentRegistry {
 public:
  ~ComponentRegistry() override = default;
  template <typename... Args>
  T& createComponent(size_t entity_id, Args... args) {
    T* component = m_allocator->makeNew<T>(std::forward<Args>(args)...);
    component->m_entity_id = entity_id;
    m_components.push_back(component);
    return *component;
  }

  void removeComponent(size_t entity_id) {
    auto it =
        std::find_if(m_components.begin(), m_components.end(), [entity_id](const IComponent* component) { return component->getEntityId() == entity_id; });
    if (it != m_components.end()) {
      m_allocator->free(*it);
      m_components.erase(it);
    }
  }

  std::optional<std::reference_wrapper<T>> getComponent(size_t entity_id) const {
    auto it =
        std::find_if(m_components.begin(), m_components.end(), [entity_id](const IComponent* component) { return component->getEntityId() == entity_id; });
    if (it != m_components.end()) {
      return std::ref(**it);
    }
    return std::nullopt;
  }

  auto getComponents() const {
    return m_components | std::views::transform([](T* c) -> T& { return *c; });
  }

 private:
  std::vector<T*> m_components;
  std::unique_ptr<memory::IAllocator> m_allocator = std::make_unique<memory::ChunkAllocator<T>>(100);
};

export class ComponentManager {
 public:
  ComponentManager() = default;

  ~ComponentManager() = default;

  template <IsComponent T, class... Args>
  void addComponent(size_t entity_id, Args... args) {
    const ComponentTypeId component_type_id = T::getComponentTypeId();
    if (const auto& it = m_component_registry_lookup.find(component_type_id); it == m_component_registry_lookup.end()) {
      m_component_registry_lookup[component_type_id] = std::make_unique<ComponentRegistry<T>>();
    }
    auto& component_handle = *dynamic_cast<ComponentRegistry<T>*>(m_component_registry_lookup[component_type_id].get());
    if (m_component_lookup_table.getComponent<T>(entity_id).has_value()) {
      throw error::Error(error::ErrorType::kEcsError, "ComponentTypeId '{}' already exists with this EntityId '{}'", component_type_id, entity_id);
    }
    auto& component = component_handle.createComponent(entity_id, std::forward<Args>(args)...);
    m_component_lookup_table.addComponent<T>(entity_id, &component);
  }

  template <IsComponent T>
  void removeComponent(const size_t entity_id) {
    m_component_lookup_table.removeComponent<T>(entity_id);
    const ComponentTypeId component_type_id = T::getComponentTypeId();
    auto& component_handle = *dynamic_cast<ComponentRegistry<T>*>(m_component_registry_lookup[component_type_id].get());
    component_handle.removeComponent(entity_id);
  }

  template <IsComponent T>
  std::optional<std::reference_wrapper<T>> getComponent(const size_t entity_id) const {
    return m_component_lookup_table.getComponent<T>(entity_id);
  }

  template <IsComponent... Ts>
  auto getComponents(const size_t entity_id) const {
    return m_component_lookup_table.getComponents<Ts...>(entity_id);
  }

  template <IsComponent T>
  auto components() const {
    return getComponentRegistry<T>().getComponents();
  }

 private:
  template <IsComponent T>
  ComponentRegistry<T>& getComponentRegistry() const {
    const ComponentTypeId component_type_id = T::getComponentTypeId();
    const auto& it = m_component_registry_lookup.find(component_type_id);
    if (it == m_component_registry_lookup.end()) {
      throw error::Error(error::ErrorType::kEcsError, "ComponentTypeId '{}' does not exists", component_type_id);
    }
    return *dynamic_cast<ComponentRegistry<T>*>(it->second.get());
  }
  ComponentLookupTable m_component_lookup_table;
  std::unordered_map<ComponentTypeId, std::unique_ptr<IComponentRegistry>> m_component_registry_lookup;
};
}  // namespace game_engine::ecs