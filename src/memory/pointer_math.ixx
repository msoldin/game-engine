module;
#include <cassert>
#include <memory>
export module vulkan_engine.memory:pointer_math;

export namespace vulkan_engine::memory::pointer_math {
inline void* alignForward(void* address, const uintptr_t alignment) {
  assert((alignment & (alignment - 1)) == 0 && "Alignment must be a power of two");

  return reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(address) + (alignment - 1)) & ~(alignment - 1));
}

inline size_t alignForwardAdjustment(const void* address, const uintptr_t alignment) {
  assert((alignment & (alignment - 1)) == 0 && "Alignment must be a power of two");

  const size_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & alignment - 1);
  if (adjustment == alignment) {
    return 0;
  }
  return adjustment;
}

inline size_t alignForwardAdjustmentWithHeader(const void* address, const uintptr_t alignment, const uintptr_t headerSize) {
  assert((alignment & (alignment - 1)) == 0 && "Alignment must be a power of two");

  size_t adjustment = alignForwardAdjustment(address, alignment);

  if (size_t needed_space = headerSize; adjustment < needed_space) {
    needed_space -= adjustment;

    // Increase adjustment to fit header
    adjustment += alignment * (needed_space / alignment);

    if (needed_space % alignment > 0) {
      adjustment += alignment;
    }
  }

  return adjustment;
}

inline void* add(const void* p, const uintptr_t x) { return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(p) + x); }

inline void* subtract(const void* p, const uintptr_t x) { return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(p) - x); }

inline uintptr_t nextPowerOfTwo(const uintptr_t value) { return std::bit_ceil(value); }
}  // namespace vulkan_engine::memory::pointer_math