module;
#include <unistd.h>

#include <cassert>

export module vulkan_engine.memory:linear_allocator;

import :allocator;
import :pointer_math;

namespace vulkan_engine::memory {
export class LinearAllocator final : public Allocator {
 public:
  explicit LinearAllocator(const size_t size) : Allocator(size), m_currentPos(m_memoryStart) {}

 private:
  void* allocate(const size_t size, const size_t alignment) override {
    // calculate adjustment needed to keep object correctly aligned
    const size_t adjustment = pointer_math::alignForwardAdjustment(m_currentPos, alignment);
    // check if there is enough space left
    if (m_spaceLeft - size - adjustment < size) {
      return nullptr;
    }
    // adjust p by adjustment
    void* result = pointer_math::add(m_currentPos, adjustment);
    // update current position
    m_currentPos = pointer_math::add(result, size);
    m_spaceLeft -= size + adjustment;
    m_numAllocations++;
    return result;
  }

  void deallocate(void* /*p*/) override { assert(false && "Use clear() to deallocate memory in LinearAllocator"); }

  void* m_currentPos;
};

}  // namespace vulkan_engine::memory