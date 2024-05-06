//
// Created by marvi on 05.05.2024.
//

#ifndef STACK_ALLOCATOR_HPP
#define STACK_ALLOCATOR_HPP
#include "allocator.hpp"

namespace vulkan_engine::memory {

class StackAllocator : public Allocator {
 public:
  StackAllocator(size_t memorySize, void* memoryStart);

 private:
  void* allocate(size_t size, size_t alignment) override;
  void deallocate(void* p) override;
  void* m_currentPos;
  void* m_head;

  struct AllocationHeader {
    size_t adjustment;
    size_t size;
  };
};

}  // namespace vulkan_engine::memory

#endif  //STACK_ALLOCATOR_HPP
