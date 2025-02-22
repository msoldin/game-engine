//
// Created by marvi on 04.05.2024.
//
module;
#include <cstddef>
#include <optional>
#include <utility>
export module vulkan_engine.ecs:entity;

import :component;
import :component_manager;

namespace vulkan_engine::ecs {
export class Entity final {
  friend class EntityManager;
  ComponentManager* m_component_manager = nullptr;

 protected:
  std::size_t m_entity_id = 0;

 public:
  size_t getId() const { return m_entity_id; }
  template <IsComponent T, class... Args>
  void addComponent(Args... args) {
    this->m_component_manager->addComponent<T>(m_entity_id, std::forward<T>(args)...);
  }
  template <IsComponent T>
  void removeComponent() {
    this->m_component_manager->removeComponent<T>(m_entity_id);
  }
  template<IsComponent T>
  std::optional<std::reference_wrapper<T>> getComponent() {
    return this->m_component_manager->getComponent<T>(m_entity_id);
  }
  template <IsComponent... Ts>
  auto getComponents() const {
    return this->m_component_manager->getComponents<Ts...>(m_entity_id);
  }
};
}  // namespace vulkan_engine::ecs