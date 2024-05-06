//
// Created by marvi on 05.05.2024.
//

#include "memory/pool_allocator.hpp"
#include "memory/pointer_math.inl"

namespace vulkan_engine::memory {

PoolAllocator::PoolAllocator(size_t memorySize, void* memoryStart,
                             size_t objectSize, size_t objectAlignment)
    : Allocator(memorySize, memoryStart),
      m_objectSize(objectSize),
      m_objectAlignment(objectAlignment) {

  size_t adjustment =
      pointer_math::alignForwardAdjustment(memoryStart, objectAlignment);
  m_freeList = static_cast<void**>(pointer_math::add(memoryStart, adjustment));
  size_t numBlocks = (memorySize - adjustment) / objectSize;
  void** p = m_freeList;

  for (size_t i = 0; i < numBlocks - 1; i++) {
    *p = pointer_math::add(p, objectSize);
    p = static_cast<void**>(*p);
  }

  *p = nullptr;
}

void* PoolAllocator::allocate(size_t size, size_t alignment) {

  if (size != m_objectSize || alignment != m_objectAlignment) {
    throw std::logic_error(
        "PoolAllocator can only allocate objects of the same size and "
        "alignment.");
  }

  if (this->m_freeList == nullptr)
    return nullptr;

  void* result = this->m_freeList;

  m_freeList = static_cast<void**>(*this->m_freeList);
  m_spaceLeft -= size;
  m_numAllocations++;

  return result;
}

void PoolAllocator::deallocate(void* p) {
  *static_cast<void**>(p) = m_freeList;
  m_freeList = static_cast<void**>(p);
  m_spaceLeft += m_objectSize;
  m_numAllocations--;
}

}  // namespace vulkan_engine::memory