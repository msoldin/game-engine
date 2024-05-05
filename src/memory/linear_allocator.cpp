//
// Created by marvi on 04.05.2024.
//

#include <memory/linear_allocator.hpp>

namespace vulkan_engine::memory {
LinearAllocator::LinearAllocator(size_t size, void* start)
    : Allocator(size, start), m_currentPos(start) {}

void* LinearAllocator::allocate(size_t size, size_t alignment) {
  if (std::align(alignment, size, m_currentPos, m_spaceLeft)) {
    void* result = m_currentPos;
    m_currentPos += size;
    m_spaceLeft -= size;
    m_numAllocations++;
    return result;
  }
  return nullptr;
}

void LinearAllocator::deallocate(void* p) {
  throw std::logic_error("LinearAllocator does not support deallocation");
}

}  // namespace vulkan_engine::memory
