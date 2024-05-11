//
// Created by marvi on 08.05.2024.
//

#ifndef SYSTEM_BUILDER_HPP
#define SYSTEM_BUILDER_HPP
#include <functional>

#include "system.hpp"
#include "memory/allocator.hpp"

namespace vulkan_engine::ecs {
class ISystemCreator {
public:
  virtual ~ISystemCreator() = default;

  virtual size_t getSize() = 0;

  virtual System* create(memory::Allocator* allocator) = 0;

};

template <typename T>
class SystemCreator final : public ISystemCreator {
public:
  template <typename... Args>
  explicit SystemCreator(Args&&... args) {
    m_create = [args...](memory::Allocator* allocator) mutable {
      return allocator->makeNew<T>(args...);
    };
  }

  ~SystemCreator() override = default;

  size_t getSize() override {
    //Object size+alignment is important for the allocator
    return sizeof(T) + alignof(T);
  }

  System* create(memory::Allocator* allocator) override {
    return m_create(allocator);
  }

private:
  std::function<T*(memory::Allocator*)> m_create;
};
} // namespace vulkan_engine::ecs

#endif  //SYSTEM_BUILDER_HPP