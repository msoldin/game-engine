//
// Created by marvi on 04.05.2024.
//

module;

#include <cstddef>

export module vulkan_engine.ecs:component;

namespace vulkan_engine::ecs {
using ComponentTypeId = std::size_t;

namespace {
ComponentTypeId generateComponentTypeId() {
  static ComponentTypeId last_id = 0;
  return last_id++;
}
}  // namespace

class IComponent {
  friend class ComponentManager;

 public:
  virtual ~IComponent() = default;

 private:
  size_t m_entity_id = 0;
};

export template <size_t MaxComponents>
class Component : public IComponent {
 public:
  static ComponentTypeId getComponentTypeId() {
    // static only get initialized once, the first time this function is called
    // each template type will have its own static variable
    static ComponentTypeId const kTypeId = generateComponentTypeId();
    return kTypeId;
  }
  static constexpr size_t getMaxComponents() { return MaxComponents; }
};
}  // namespace vulkan_engine::ecs