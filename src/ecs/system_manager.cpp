//
// Created by marvi on 04.05.2024.
//

#include <ecs/system_manager.hpp>

#include "memory/pointer_math.inl"

namespace vulkan_engine::ecs {

SystemManager::SystemManager(std::vector<std::unique_ptr<ISystemCreator> >& systemCreators) {
  //Calculate the size of each ISystem Objects
  size_t systemSize = 0;
  for (const auto& creator : systemCreators) {
    systemSize += creator->getSize();
  }
  systemSize = memory::pointer_math::nextPowerOfTwo(systemSize);
  m_systemAllocator = new memory::LinearAllocator(systemSize);
  m_systems.reserve(systemCreators.size());

  for (const auto& creator : systemCreators) {
    m_systems.push_back(creator->create(m_systemAllocator));
  }
}

SystemManager::~SystemManager() {
  delete m_systemAllocator;
}

void SystemManager::update(const uint64_t dt) {
  for (auto system : m_systems) {
    system->preUpdate(dt);
  }
  for (auto system : m_systems) {
    system->update(dt);
  }
  for (auto system : m_systems) {
    system->postUpdate(dt);
  }
}

}