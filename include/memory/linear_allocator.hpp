//
// Created by marvi on 04.05.2024.
//

#ifndef LINEAR_ALLOCATOR_HPP
#define LINEAR_ALLOCATOR_HPP
#include "allocator.hpp"

namespace vulkan_engine::memory {

class LinearAllocator final : public Allocator {

public:
  explicit LinearAllocator(size_t size);

private:
  void* allocate(size_t size, size_t alignment) override;

  void deallocate(void* p) override;

  void* m_currentPos;
};

} // namespace vulkan_engine::memory

#endif  //LINEAR_ALLOCATOR_HPP