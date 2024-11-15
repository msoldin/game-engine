//
// Created by marvi on 03.05.2024.
//
module;

#include <memory>
#include <vector>

export module vulkan_engine.ecs:coordinator;

import :system;
import :system_manager;
import :entity_manager;
import :component_manager;

namespace vulkan_engine::ecs {

export class CoordinatorBuilder;

export class Coordinator {
  friend class CoordinatorBuilder;

 public:
  void update(const uint64_t dt) const { m_systemManager->update(dt); };

 private:
  explicit Coordinator(std::vector<std::unique_ptr<ISystemCreator>>& systemCreators) {
    m_systemManager = std::make_unique<SystemManager>(systemCreators);
    m_entityManager = std::make_unique<EntityManager>();
    m_componentManager = std::make_unique<ComponentManager>();
  };
  std::unique_ptr<EntityManager> m_entityManager;
  std::unique_ptr<SystemManager> m_systemManager;
  std::unique_ptr<ComponentManager> m_componentManager;
};

class CoordinatorBuilder final {
 public:
  CoordinatorBuilder() = default;

  ~CoordinatorBuilder() = default;

  CoordinatorBuilder(CoordinatorBuilder&&) = delete;

  CoordinatorBuilder(const CoordinatorBuilder&) = delete;

  CoordinatorBuilder& operator=(CoordinatorBuilder&&) = delete;

  CoordinatorBuilder& operator=(const CoordinatorBuilder&) = delete;

  void registerSystem(std::unique_ptr<ISystemCreator> systemCreator) { m_systemCreators.push_back(std::move(systemCreator)); }

  std::unique_ptr<Coordinator> build() { return std::unique_ptr<Coordinator>(new Coordinator(m_systemCreators)); }

 private:
  std::vector<std::unique_ptr<ISystemCreator>> m_systemCreators;
};
}  // namespace vulkan_engine::ecs