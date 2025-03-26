//
// Created by marvi on 04.05.2024.
//
module;
#include <memory>
#include <vector>

import vulkan_engine.memory;

export module vulkan_engine.ecs:system_manager;

import :system;
import :component_manager;
import :entity_manager;

namespace vulkan_engine::ecs {

export class SystemManager final {
 public:
  explicit SystemManager() {}

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

  template <class T, typename... Args>
  SystemManager& add_system(Args&&... args) {
    m_systems.push_back(new T(args...));
    return *this;
  }

 private:
  std::vector<System*> m_systems;
  std::unique_ptr<memory::LinearAllocator> m_system_allocator;
};

}  // namespace vulkan_engine::ecs