//
// Created by marvi on 05.05.2024.
//

#ifndef POOL_ALLOCATOR_HPP
#define POOL_ALLOCATOR_HPP
#include "allocator.hpp"

namespace vulkan_engine::memory {

class PoolAllocator : public Allocator {
 public:
  PoolAllocator(size_t memorySize, void* memoryStart, size_t objectSize,
                size_t objectAlignment);

 private:
  void* allocate(size_t size, size_t alignment) override;
  void deallocate(void* p) override;
  void** m_freeList;
  size_t m_objectSize;
  size_t m_objectAlignment;
};

}  // namespace vulkan_engine::memory

#endif  //POOL_ALLOCATOR_HPP
