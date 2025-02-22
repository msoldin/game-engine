//
// Created by marvi on 03.05.2024.
//
module;
#include <cstddef>
import vulkan_engine.memory;
export module vulkan_engine.ecs:entity_manager;

import :entity;
import :component_manager;

namespace vulkan_engine::ecs {
export class EntityManager {
 public:
  explicit EntityManager() = default;

  Entity* createEntity(ComponentManager* component_manager) {
    auto* entity = new Entity();
    entity->m_component_manager = component_manager;
    entity->m_entity_id = m_entity_count++;
    return entity;
  }

 private:
  size_t m_entity_count{0};
};
}  // namespace vulkan_engine::ecs