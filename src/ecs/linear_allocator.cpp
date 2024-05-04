//
// Created by marvi on 04.05.2024.
//

#include <ecs/linear_allocator.hpp>

namespace vulkan_engine::ecs {
LinearAllocator::LinearAllocator(size_t size, void* start)
    : Allocator(size, start),
      m_start(start),
      m_spaceLeft(size),
      m_currentPos(start) {}

void* LinearAllocator::allocate(size_t size, size_t alignment) {
  if (std::align(alignment, size, m_currentPos, m_spaceLeft)) {
    void* result = m_currentPos;
    m_currentPos = reinterpret_cast<void*>(reinterpret_cast<size_t>(m_currentPos) + size);
    m_spaceLeft -= size;
    return result;
  }
  return nullptr;
}

}  // namespace vulkan_engine::ecs
