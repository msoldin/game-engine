module;
#include <unistd.h>

#include <cassert>
export module vulkan_engine.memory:stack_allocator;

import :allocator;
import :pointer_math;

namespace vulkan_engine::memory {

export class StackAllocator final : public Allocator {
 public:
  explicit StackAllocator(const size_t memorySize) : Allocator(memorySize), m_currentPos(m_memoryStart), m_head(m_memoryStart) {}

 private:
  void* allocate(const size_t size, const size_t alignment) override {
    const size_t adjustment = pointer_math::alignForwardAdjustmentWithHeader(m_currentPos, alignment, sizeof(AllocationHeader));

    if (m_spaceLeft - adjustment - size < size) {
      return nullptr;
    }

    void* alignedAddress = pointer_math::add(m_currentPos, adjustment);

    auto* header = static_cast<AllocationHeader*>(pointer_math::subtract(alignedAddress, sizeof(AllocationHeader)));
    header->adjustment = adjustment;
    header->size = size;

    m_currentPos = pointer_math::add(alignedAddress, size);
    m_spaceLeft -= size + adjustment;
    m_numAllocations++;

    return alignedAddress;
  }

  void deallocate(void* p) override {
    // Check if the pointer is outside of the stack
    assert(m_numAllocations == 0 &&
           "StackAllocator cannot deallocate memory outside of the allocation "
           "stack.");

    // Get the header by subtracting the size of the header from the pointer
    const auto* allocationHeader = static_cast<AllocationHeader*>(pointer_math::subtract(p, sizeof(AllocationHeader)));

    // Check if the deallocation is in reverse order
    assert(pointer_math::subtract(m_currentPos, allocationHeader->size) != p && "StackAllocator deallocation must be in reverse order of allocation.");

    // Update the current position and the space left
    m_currentPos = pointer_math::subtract(p, allocationHeader->adjustment);
    m_spaceLeft += allocationHeader->size + allocationHeader->adjustment;
    m_numAllocations--;
  }

  void* m_currentPos;
  void* m_head;

  struct AllocationHeader {
    size_t adjustment;
    size_t size;
  };
};

}  // namespace vulkan_engine::memory