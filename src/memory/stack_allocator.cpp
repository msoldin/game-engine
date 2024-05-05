//
// Created by marvi on 05.05.2024.
//

#include "memory/stack_allocator.hpp"

#include <glog/logging.h>

#include "core/application.h"

namespace vulkan_engine::memory {

StackAllocator::StackAllocator(size_t memorySize, void* memoryStart)
    : Allocator(memorySize, memoryStart),
      m_currentPos(memoryStart),
      m_head(memoryStart) {}

void* StackAllocator::allocate(size_t size, size_t alignment) {
  size_t padding = size + sizeof(AllocationHeader);
  if (std::align(alignment, padding, m_currentPos, m_spaceLeft)) {
    auto* header = static_cast<AllocationHeader*>(m_currentPos);
    header->padding = padding;
    void* result = m_currentPos + sizeof(AllocationHeader);
    m_currentPos += padding;
    m_spaceLeft -= padding;
    m_numAllocations++;
    return result;
  }
  return nullptr;
}

void StackAllocator::deallocate(void* p) {
  void* headerAdress = p - sizeof(AllocationHeader);
  auto* header = static_cast<AllocationHeader*>(headerAdress);
  void* blockStart = m_currentPos - header->padding + sizeof(AllocationHeader);
  if (blockStart == p) {
    m_currentPos -= header->padding;
    m_spaceLeft += header->padding;
    m_numAllocations--;
  } else {
    throw std::logic_error(
        "StackAllocator does not support deallocation of arbitrary memory "
        "locations");
  }
}

}  // namespace vulkan_engine::memory
