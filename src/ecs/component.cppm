//
// Created by marvi on 04.05.2024.
//

module;

#include <cstddef>

export module vulkan_engine.ecs:component;

namespace vulkan_engine::ecs {
using ComponentTypeId = std::size_t;

namespace {
ComponentTypeId generateComponentTypeId() {
  static ComponentTypeId lastId = 0;
  return lastId++;
}
}  // namespace

class Component {
 public:
  template <typename T>
  static ComponentTypeId getComponentTypeId() {
    // static only get initialized once, the first time this function is called
    // each templaty type will have its own static variable
    static ComponentTypeId const kTypeId = generateComponentTypeId();
    return kTypeId;
  }
};
}  // namespace vulkan_engine::ecs