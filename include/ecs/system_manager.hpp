//
// Created by marvi on 04.05.2024.
//

#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H
#include <vector>

#include "memory/linear_allocator.hpp"
#include "system_builder.inl"

namespace vulkan_engine::ecs {

class SystemManager final {

  friend class Coordinator;

public:
  explicit SystemManager(std::vector<std::unique_ptr<ISystemCreator> >& systemCreators);

  ~SystemManager();

  SystemManager(SystemManager&&) = delete;

  SystemManager(const SystemManager&) = delete;

  SystemManager& operator=(SystemManager&&) = delete;

  SystemManager& operator=(const SystemManager&) = delete;

private:
  void update(uint64_t dt);

  std::vector<System*> m_systems;
  memory::LinearAllocator* m_systemAllocator;
};

} // namespace vulkan_engine::ecs

#endif  //SYSTEM_MANAGER_H