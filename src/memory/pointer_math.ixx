module;
#include <memory>
export module vulkan_engine.memory:pointer_math;

export namespace vulkan_engine::memory::pointer_math {
inline void* alignForward(void* address, const size_t alignment) {
  return reinterpret_cast<void*>(reinterpret_cast<size_t>(address) + (alignment - 1) & ~(alignment - 1));
}

inline size_t alignForwardAdjustment(const void* address, const size_t alignment) {
  const size_t adjustment = alignment - (reinterpret_cast<size_t>(address) & alignment - 1);
  if (adjustment == alignment) {
    return 0;
  }
  return adjustment;
}

inline size_t alignForwardAdjustmentWithHeader(const void* address, const size_t alignment, const size_t headerSize) {
  size_t adjustment = alignForwardAdjustment(address, alignment);

  if (size_t neededSpace = headerSize; adjustment < neededSpace) {
    neededSpace -= adjustment;

    // Increase adjustment to fit header
    adjustment += alignment * (neededSpace / alignment);

    if (neededSpace % alignment > 0) {
      adjustment += alignment;
    }
  }

  return adjustment;
}

inline void* add(const void* p, const size_t x) { return reinterpret_cast<void*>(reinterpret_cast<size_t>(p) + x); }

inline void* subtract(const void* p, const size_t x) { return reinterpret_cast<void*>(reinterpret_cast<size_t>(p) - x); }

inline size_t nextPowerOfTwo(const size_t value) { return std::bit_ceil(value); }
}  // namespace vulkan_engine::memory::pointer_math