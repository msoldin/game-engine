//
// Created by marvi on 03.05.2024.
//

#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <memory>

#include "component_manager.hpp"
#include "entity_manager.h"
#include "system_manager.hpp"

namespace vulkan_engine::ecs {
class Coordinator final {
 private:
  std::unique_ptr<EntityManager> m_entityManager;
  std::unique_ptr<SystemManager> m_systemManager;
  std::unique_ptr<ComponentManager> m_compomnentManager;
};
}  // namespace vulkan_engine::ecs

#endif  //COORDINATOR_H
