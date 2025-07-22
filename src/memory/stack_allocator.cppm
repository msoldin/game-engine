module;
#include <unistd.h>

#include <cassert>
export module game_engine.memory:stack_allocator;

import :allocator;
import :pointer_math;

namespace game_engine::memory {

export class StackAllocator final : public Allocator {
 public:
  explicit StackAllocator(const size_t memorySize) : Allocator(memorySize), m_current_pos(m_memory_start), m_head(m_memory_start) {}

  void* allocate(const size_t size, const size_t alignment) override {
    const size_t adjustment = pointer_math::alignForwardAdjustmentWithHeader(m_current_pos, alignment, sizeof(AllocationHeader));

    if (size + adjustment > m_space_left) {
      return nullptr;
    }

    void* aligned_address = pointer_math::add(m_current_pos, adjustment);

    auto* header = static_cast<AllocationHeader*>(pointer_math::subtract(aligned_address, sizeof(AllocationHeader)));
    header->adjustment = adjustment;
    header->size = size;

    m_current_pos = pointer_math::add(aligned_address, size);
    m_space_left -= size + adjustment;
    m_num_allocations++;

    return aligned_address;
  }

  void free(void* p) override {
    // Check if the pointer is outside the stack
    assert(m_num_allocations > 0 &&
           "StackAllocator cannot deallocate memory outside of the allocation "
           "stack.");

    // Get the header by subtracting the size of the header from the pointer
    const auto* allocation_header = static_cast<AllocationHeader*>(pointer_math::subtract(p, sizeof(AllocationHeader)));

    // Check if the deallocation is in reverse order
    assert(pointer_math::subtract(m_current_pos, allocation_header->size) == p && "StackAllocator deallocation must be in reverse order of allocation.");

    // Update the current position and the space left
    m_current_pos = pointer_math::subtract(p, allocation_header->adjustment);
    m_space_left += allocation_header->size + allocation_header->adjustment;
    m_num_allocations--;
  }

 private:
  void* m_current_pos;
  void* m_head;

  struct AllocationHeader {
    size_t adjustment;
    size_t size;
  };
};

}  // namespace game_engine::memory