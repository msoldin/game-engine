//
// Created by marvi on 04.05.2024.
//

#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <cstddef>

namespace vulkan_engine::ecs {
using ComponentTypeId = std::size_t;

inline ComponentTypeId generateComponentTypeId() {
  static ComponentTypeId lastId = 0;
  return lastId++;
}

class Component {
public:
  template <typename T>
  static ComponentTypeId getComponentTypeId() {
    //static only get initialized once, the first time this function is called
    //each templaty type will have its own static variable
    static ComponentTypeId typeId = generateComponentTypeId();
    return typeId;
  }
};
} // namespace vulkan_engine::ecs

#endif  //COMPONENT_HPP