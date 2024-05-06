//
// Created by marvi on 06.05.2024.
//

#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <cstdint>

namespace vulkan_engine::ecs {

class System {

 public:
  virtual ~System() = default;
  virtual void preUpdate(uint64_t dt);
  virtual void update(uint64_t dt);
  virtual void postUpdate(uint64_t dt);
};

}  // namespace vulkan_engine::ecs

#endif  //SYSTEM_HPP
