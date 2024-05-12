//
// Created by marvi on 05.05.2024.
//

#include "memory/pool_allocator.hpp"

#include <cassert>

#include "memory/pointer_math.inl"

namespace vulkan_engine::memory {
PoolAllocator::PoolAllocator(const size_t memorySize, const size_t objectSize, const size_t objectAlignment)
  : Allocator(memorySize),
    m_objectSize(objectSize),
    m_objectAlignment(objectAlignment) {
  const size_t adjustment =
      pointer_math::alignForwardAdjustment(m_memoryStart, objectAlignment);
  m_freeList = static_cast<void**>(pointer_math::add(m_memoryStart, adjustment));
  const size_t numBlocks = (memorySize - adjustment) / objectSize;
  void** p = m_freeList;

  for (size_t i = 0; i < numBlocks - 1; i++) {
    *p = pointer_math::add(p, objectSize); // NOLINT(*-multi-level-implicit-pointer-conversion)
    p = static_cast<void**>(*p);
  }

  *p = nullptr;
}

void* PoolAllocator::allocate(const size_t size, const size_t alignment) {
  assert(size == m_objectSize
      && alignment == m_objectAlignment
      && "PoolAllocator can only allocate objects of the same size and alignment.");

  if (this->m_freeList == nullptr) {
    return nullptr;
  }

  void* result = this->m_freeList; // NOLINT(*-multi-level-implicit-pointer-conversion)

  m_freeList = static_cast<void**>(*this->m_freeList);
  m_spaceLeft -= size;
  m_numAllocations++;

  return result;
}

void PoolAllocator::deallocate(void* p) {
  *static_cast<void**>(p) = m_freeList; // NOLINT(*-multi-level-implicit-pointer-conversion)
  m_freeList = static_cast<void**>(p);
  m_spaceLeft += m_objectSize;
  m_numAllocations--;
}
} // namespace vulkan_engine::memory