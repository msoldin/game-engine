//
// Created by marvi on 03.05.2024.
//
module;

#include <memory>
#include <vector>

import vulkan_engine.memory;

export module vulkan_engine.ecs:coordinator;

import :system;
import :system_manager;
import :entity_manager;
import :component_manager;
import :entity;

namespace vulkan_engine::ecs {
export class CoordinatorBuilder;

export class Coordinator {
 public:
  void update(const uint64_t dt) const { m_system_manager->update(dt); };

  ComponentManager& getComponentManager() const { return *m_component_manager; }
  EntityManager& getEntityManager() const { return *m_entity_manager; }
  SystemManager& getSystemManager() const { return *m_system_manager; }
  Entity* createEntity() const { return m_entity_manager->createEntity(m_component_manager.get()); }

 private:
  explicit Coordinator() {
    m_entity_manager = std::make_unique<EntityManager>();
    m_component_manager = std::make_unique<ComponentManager>();
    m_system_manager = std::make_unique<SystemManager>();
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
}  // namespace vulkan_engine::ecs