//
// Created by marvi on 04.05.2024.
//
module;
#include <memory>
#include <vector>

import vulkan_engine.memory;

export module vulkan_engine.ecs:system_manager;

import :system;

namespace vulkan_engine::ecs {

export class SystemManager final {
 public:
  explicit SystemManager(std::vector<std::unique_ptr<ISystemCreator>>& systemCreators) {
    // Calculate the size of each ISystem Objects
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

  ~SystemManager() { delete m_systemAllocator; }

  SystemManager(SystemManager&&) = delete;

  SystemManager(const SystemManager&) = delete;

  SystemManager& operator=(SystemManager&&) = delete;

  SystemManager& operator=(const SystemManager&) = delete;

  void update(const uint64_t dt) {
    for (auto* system : m_systems) {
      system->preUpdate(dt);
    }
    for (auto* system : m_systems) {
      system->update(dt);
    }
    for (auto* system : m_systems) {
      system->postUpdate(dt);
    }
  }

 private:
  std::vector<System*> m_systems;
  memory::LinearAllocator* m_systemAllocator;
};

}  // namespace vulkan_engine::ecs