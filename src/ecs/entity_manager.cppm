module;
#include <vector>
export module game_engine.ecs:entity_manager;

import :entity;
import :component_manager;

import game_engine.memory;

namespace game_engine::ecs {
export class EntityManager {
 public:
  explicit EntityManager(ComponentManager* component_manager) : m_component_manager(component_manager) {}

  Entity* createEntity() {
    auto* entity = new Entity();
    entity->m_component_manager = m_component_manager;
    entity->m_entity_id = m_entity_count++;
    m_entities.push_back(entity);
    return entity;
  }

  ~EntityManager() {
    for (const Entity* entity : m_entities) {
      delete entity;  // Delete each Entity pointer
    }
    m_entities.clear();  // Clear the vector
  }

 private:
  size_t m_entity_count{0};
  std::vector<Entity*> m_entities;
  ComponentManager* m_component_manager;
};
}  // namespace game_engine::ecs