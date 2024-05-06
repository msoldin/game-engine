//
// Created by marvi on 05.05.2024.
//

#include "memory/stack_allocator.hpp"

#include "memory/pointer_math.inl"

namespace vulkan_engine::memory {

StackAllocator::StackAllocator(size_t memorySize, void* memoryStart)
    : Allocator(memorySize, memoryStart),
      m_currentPos(memoryStart),
      m_head(memoryStart) {}

void* StackAllocator::allocate(size_t size, size_t alignment) {
  size_t adjustment = pointer_math::alignForwardAdjustmentWithHeader(
      m_currentPos, alignment, sizeof(AllocationHeader));

  if (m_spaceLeft - adjustment - size < size)
    return nullptr;

  void* alignedAddress = pointer_math::add(m_currentPos, adjustment);

  auto* header = static_cast<AllocationHeader*>(
      pointer_math::subtract(alignedAddress, sizeof(AllocationHeader)));
  header->adjustment = adjustment;
  header->size = size;

  m_currentPos = pointer_math::add(alignedAddress, size);
  m_spaceLeft -= size + adjustment;
  m_numAllocations++;

  return alignedAddress;
}

void StackAllocator::deallocate(void* p) {
  // Check if the pointer is outside of the stack
  if (m_numAllocations == 0) {
    throw std::logic_error(
        "StackAllocator cannot deallocate memory outside of the allocation "
        "stack.");
  }

  // Get the header by subtracting the size of the header from the pointer
  auto* allocationHeader = static_cast<AllocationHeader*>(
      pointer_math::subtract(p, sizeof(AllocationHeader)));

  // Check if the deallocation is in reverse order
  void* alignedOldPos =
      pointer_math::subtract(m_currentPos, allocationHeader->size);
  if (alignedOldPos != p) {
    throw std::logic_error(
        "StackAllocator deallocation must be in reverse order of allocation.");
  }

  // Update the current position and the space left
  m_currentPos = pointer_math::subtract(p, allocationHeader->adjustment);
  m_spaceLeft += allocationHeader->size + allocationHeader->adjustment;
  m_numAllocations--;
}

}  // namespace vulkan_engine::memory
