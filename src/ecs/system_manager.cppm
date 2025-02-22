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
  explicit SystemManager(const std::vector<std::unique_ptr<ISystemCreator>>& system_creators) {
    // Calculate the size of each ISystem Objects
    size_t system_size = 0;
    for (const auto& creator : system_creators) {
      system_size += creator->getSize();
    }
    system_size = memory::pointer_math::nextPowerOfTwo(system_size);
    m_system_allocator = std::make_unique<memory::LinearAllocator>(system_size);
    m_systems.reserve(system_creators.size());

    for (const auto& creator : system_creators) {
      m_systems.push_back(creator->create(*m_system_allocator));
    }
  }

  SystemManager(SystemManager&&) = delete;

  SystemManager(const SystemManager&) = delete;

  SystemManager& operator=(SystemManager&&) = delete;

  SystemManager& operator=(const SystemManager&) = delete;

  void update(const uint64_t dt) const {
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
  std::unique_ptr<memory::LinearAllocator> m_system_allocator;
};

}  // namespace vulkan_engine::ecs