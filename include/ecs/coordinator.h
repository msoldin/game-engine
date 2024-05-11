//
// Created by marvi on 03.05.2024.
//

#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <memory>

#include "entity_manager.h"
#include "system_manager.hpp"
#include "component_manager.hpp"

namespace vulkan_engine::ecs {

class Coordinator final {
  friend class CoordinatorBuilder;

public:
  void update(uint64_t dt) const;

private:
  explicit Coordinator(std::vector<std::unique_ptr<ISystemCreator> >& systemCreators);

  std::unique_ptr<EntityManager> m_entityManager;
  std::unique_ptr<SystemManager> m_systemManager;
  std::unique_ptr<ComponentManager> m_compomnentManager;

};
} // namespace vulkan_engine::ecs

#endif  //COORDINATOR_H