//
// Created by marvi on 05.05.2024.
//

#ifndef POINTER_MATH_HPP
#define POINTER_MATH_HPP
#include <assert.h>

#include <limits>

namespace vulkan_engine::memory::pointer_math {

inline void* alignForward(void* address, size_t alignment) {
  return reinterpret_cast<void*>(
    reinterpret_cast<size_t>(address) + (alignment - 1) & ~(alignment - 1));
}

inline size_t alignForwardAdjustment(const void* adress, size_t alignment) {
  size_t adjustment =
      alignment - (reinterpret_cast<size_t>(adress) & alignment - 1);
  if (adjustment == alignment)
    return 0;

  //already aligned
  return adjustment;
}

inline size_t alignForwardAdjustmentWithHeader(const void* adress,
                                               size_t alignment,
                                               size_t headerSize) {
  size_t adjustment = alignForwardAdjustment(adress, alignment);
  size_t neededSpace = headerSize;

  if (adjustment < neededSpace) {
    neededSpace -= adjustment;

    //Increase adjustment to fit header
    adjustment += alignment * (neededSpace / alignment);

    if (neededSpace % alignment > 0)
      adjustment += alignment;
  }

  return adjustment;
}

inline void* add(const void* p, size_t x) {
  return reinterpret_cast<void*>(reinterpret_cast<size_t>(p) + x);
}

inline void* subtract(const void* p, size_t x) {
  return reinterpret_cast<void*>(reinterpret_cast<size_t>(p) - x);
}

inline size_t nextPowerOfTwo(const size_t value) {
  return std::bit_ceil(value);
  // assert(value != 0 && "Value must be greater than 0.");
  // assert(value <= std::numeric_limits<std::size_t>::max() / 2 + 1 && "Value is too large.");
  // std::size_t curr = value - (value != 0u);
  //
  // for (int next = 1; next < std::numeric_limits<std::size_t>::digits; next = next * 2) {
  //   curr |= curr >> next;
  // }
  //
  // return ++curr;
}

} // namespace vulkan_engine::memory::pointer_math

#endif  //POINTER_MATH_HPP