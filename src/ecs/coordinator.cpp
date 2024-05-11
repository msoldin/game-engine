//
// Created by marvi on 03.05.2024.
//

#include "ecs/coordinator.h"

namespace vulkan_engine::ecs {

Coordinator::Coordinator(std::vector<std::unique_ptr<ISystemCreator> >& systemCreators) {
  m_systemManager = std::make_unique<SystemManager>(systemCreators);
  m_entityManager = std::make_unique<EntityManager>();
  m_compomnentManager = std::make_unique<ComponentManager>();
}

void Coordinator::update(const uint64_t dt) const {
  m_systemManager->update(dt);
}

}