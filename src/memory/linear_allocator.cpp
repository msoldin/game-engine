//
// Created by marvi on 04.05.2024.
//

#include <memory/linear_allocator.hpp>

#include "memory/pointer_math.inl"

namespace vulkan_engine::memory {
LinearAllocator::LinearAllocator(const size_t size)
  : Allocator(size), m_currentPos(m_memoryStart) {}

void* LinearAllocator::allocate(const size_t size, const size_t alignment) {
  //calculate adjustment needed to keep object correctly aligned
  size_t adjustment =
      pointer_math::alignForwardAdjustment(m_currentPos, alignment);
  //check if there is enough space left
  if (m_spaceLeft - size - adjustment < size)
    return nullptr;
  //adjust p by adjustment
  void* result = pointer_math::add(m_currentPos, adjustment);
  //update current position
  m_currentPos = pointer_math::add(result, size);
  m_spaceLeft -= size + adjustment;
  m_numAllocations++;
  return result;
}

void LinearAllocator::deallocate(void* p) {
  throw std::logic_error("LinearAllocator does not support deallocation");
}

} // namespace vulkan_engine::memory