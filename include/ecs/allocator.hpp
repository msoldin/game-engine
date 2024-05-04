//
// Created by marvi on 04.05.2024.
//

#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP
#include <memory>
#include <utility>

namespace vulkan_engine::ecs {

class Allocator {

 public:
  Allocator(size_t memorySize, void* memoryStart){};
  virtual ~Allocator() = default;
  template <typename T, typename... Args>
  T* makeNew(Args&&... args) {
    return new (allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
  }

 private:
  virtual void* allocate(size_t size, size_t alignment) = 0;
  virtual void deallocate(void* p) = 0;
  virtual size_t allocated_size(void* p) = 0;
};

}  // namespace vulkan_engine::ecs

#endif  //ALLOCATOR_HPP
