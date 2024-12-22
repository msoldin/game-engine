//
// Created by marvi on 03.05.2024.
//
module;
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
    return entity;
  };

 private:
};
}  // namespace vulkan_engine::ecs