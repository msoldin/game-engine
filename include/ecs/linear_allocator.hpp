//
// Created by marvi on 04.05.2024.
//

#ifndef LINEAR_ALLOCATOR_HPP
#define LINEAR_ALLOCATOR_HPP
#include "allocator.hpp"

namespace vulkan_engine::ecs {

class LinearAllocator : public Allocator {

 public:
  LinearAllocator(size_t size, void* start);
  ~LinearAllocator() override { free(m_start); }

 private:
  void* allocate(size_t size, size_t alignment) override;
  void deallocate(void* p) override {}
  size_t allocated_size(void* p) override { return 0; }
  void* m_start;
  void* m_currentPos;
  size_t m_spaceLeft;
};

}  // namespace vulkan_engine::ecs

#endif  //LINEAR_ALLOCATOR_HPP
