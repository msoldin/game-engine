//
// Created by marvi on 10.05.2024.
//

#ifndef ECS_HPP
#define ECS_HPP
#include <memory>

#include "coordinator_builder.inl"

namespace vulkan_engine::ecs {
static std::unique_ptr<CoordinatorBuilder> create() {
  return std::make_unique<CoordinatorBuilder>();
}
}



#endif //ECS_HPP
