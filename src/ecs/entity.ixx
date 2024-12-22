//
// Created by marvi on 04.05.2024.
//
module;
#include <cstddef>
#include <utility>
export module vulkan_engine.ecs:entity;

import :component_manager;

namespace vulkan_engine::ecs {
export class Entity final {
  friend class EntityManager;

 private:
  ComponentManager* m_component_manager = nullptr;

 protected:
  std::size_t m_entity_id = 0;

 public:
  template <class T, class... Args>
  void addComponent(Args... args) {
    this->m_component_manager->addComponent<T>(m_entity_id, std::forward<T>(args)...);
  }
};
}  // namespace vulkan_engine::ecs