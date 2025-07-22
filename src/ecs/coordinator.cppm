//
// Created by marvi on 03.05.2024.
//
module;
#include <memory>
export module game_engine.ecs:coordinator;

import :system;
import :system_manager;
import :entity_manager;
import :component_manager;
import :entity;

namespace game_engine::ecs {
export class CoordinatorBuilder;

export class Coordinator {
 public:
  void update(const uint64_t dt) const { m_system_manager->update(dt); };

  EntityManager& getEntityManager() const { return *m_entity_manager; }
  SystemManager& getSystemManager() const { return *m_system_manager; }
  ComponentManager& getComponentManager() const { return *m_component_manager; }

 private:
  explicit Coordinator() {
    m_component_manager = std::make_unique<ComponentManager>();
    m_system_manager = std::make_unique<SystemManager>();
    m_entity_manager = std::make_unique<EntityManager>(m_component_manager.get());
  }
  std::unique_ptr<EntityManager> m_entity_manager;
  std::unique_ptr<SystemManager> m_system_manager;
  std::unique_ptr<ComponentManager> m_component_manager;

  friend class CoordinatorBuilder;
};

class CoordinatorBuilder final {
 public:
  CoordinatorBuilder() = default;

  ~CoordinatorBuilder() = default;

  CoordinatorBuilder(CoordinatorBuilder&&) = delete;

  CoordinatorBuilder(const CoordinatorBuilder&) = delete;

  CoordinatorBuilder& operator=(CoordinatorBuilder&&) = delete;

  CoordinatorBuilder& operator=(const CoordinatorBuilder&) = delete;

  std::unique_ptr<Coordinator> build() { return std::unique_ptr<Coordinator>(new Coordinator()); }
};
}  // namespace game_engine::ecs