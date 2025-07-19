module;
#include <unistd.h>
#include <cassert>
export module vulkan_engine.memory:linear_allocator;

import :allocator;
import :pointer_math;

namespace vulkan_engine::memory {
export class LinearAllocator final : public Allocator {
 public:
  explicit LinearAllocator(const size_t size) : Allocator(size), m_current_pos(m_memory_start) {}

  LinearAllocator(void* memory_start, const size_t memory_size) : Allocator(memory_start, memory_size), m_current_pos(m_memory_start) {}

  void* allocate(const size_t size, const size_t alignment) override {
    // calculate adjustment needed to keep object correctly aligned
    const size_t adjustment = pointer_math::alignForwardAdjustment(m_current_pos, alignment);
    // check if there is enough space left
    if (size + adjustment > m_space_left) {
      return nullptr;
    }
    // adjust p by adjustment
    void* result = pointer_math::add(m_current_pos, adjustment);
    // update current position
    m_current_pos = pointer_math::add(result, size);
    m_space_left -= size + adjustment;
    m_num_allocations++;
    return result;
  }

  void free(void* /*p*/) override { assert(false && "Use clear() to deallocate memory in LinearAllocator"); }

private:
  void* m_current_pos;
};

}  // namespace vulkan_engine::memory