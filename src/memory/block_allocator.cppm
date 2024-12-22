//
// Created by marvi on 23.11.2024.
//
module;

#include <unistd.h>

#include <cassert>

export module vulkan_engine.memory:block_allocator;

import :allocator;
import :linear_allocator;

namespace vulkan_engine::memory {
export class BlockAllocator final : public LinearAllocator {
 public:
  explicit BlockAllocator(const size_t size) : LinearAllocator(size) {}

  BlockAllocator(void* memory_start, const size_t memory_size) : LinearAllocator(memory_start, memory_size) {}

  void* allocate(const size_t size, const size_t alignment) override { return LinearAllocator::allocate(size, alignment); }
};
}  // namespace vulkan_engine::memory