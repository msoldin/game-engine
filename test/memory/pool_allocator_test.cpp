#include <gtest/gtest.h>

import vulkan_engine.memory;

struct TestStruct {
  int64_t a;
  int64_t b;
  TestStruct(const int64_t x, const int64_t y) : a(x), b(y) {}
};

class PoolAllocatorTest : public ::testing::Test {
 protected:
  static constexpr size_t OBJECT_SIZE = 16;
  static constexpr size_t OBJECT_ALIGNMENT = alignof(std::max_align_t);
  static constexpr size_t POOL_SIZE = 1024;
  std::unique_ptr<vulkan_engine::memory::PoolAllocator> m_allocator;
  std::unique_ptr<vulkan_engine::memory::PoolAllocator> m_struct_allocator;

  void SetUp() override {
    m_allocator = std::make_unique<vulkan_engine::memory::PoolAllocator>(POOL_SIZE, OBJECT_SIZE, OBJECT_ALIGNMENT);
    m_struct_allocator = vulkan_engine::memory::PoolAllocator::create<TestStruct>(64);
  }
};

TEST_F(PoolAllocatorTest, BasicAllocation) {
  void* ptr = m_allocator->allocate(OBJECT_SIZE, OBJECT_ALIGNMENT);
  EXPECT_NE(ptr, nullptr);
  EXPECT_EQ(m_allocator->getNumAllocations(), 1);
}

TEST_F(PoolAllocatorTest, MultipleAllocations) {
  void* ptr1 = m_allocator->allocate(OBJECT_SIZE, OBJECT_ALIGNMENT);
  void* ptr2 = m_allocator->allocate(OBJECT_SIZE, OBJECT_ALIGNMENT);
  EXPECT_NE(ptr1, ptr2);
  EXPECT_EQ(m_allocator->getNumAllocations(), 2);
}

TEST_F(PoolAllocatorTest, PoolExhaustion) {
  constexpr size_t num_allocations = POOL_SIZE / OBJECT_SIZE;
  std::vector<void*> allocated_ptrs;
  for (size_t i = 0; i < num_allocations; i++) {
    allocated_ptrs.push_back(m_allocator->allocate(OBJECT_SIZE, OBJECT_ALIGNMENT));
    EXPECT_NE(allocated_ptrs.back(), nullptr);
  }
  EXPECT_EQ(m_allocator->getNumAllocations(), num_allocations);

  // Next allocation should fail
  EXPECT_THROW(m_allocator->allocate(OBJECT_SIZE, OBJECT_ALIGNMENT), std::out_of_range);
}

TEST_F(PoolAllocatorTest, DeallocationAndReuse) {
  void* ptr1 = m_allocator->allocate(OBJECT_SIZE, OBJECT_ALIGNMENT);
  void* ptr2 = m_allocator->allocate(OBJECT_SIZE, OBJECT_ALIGNMENT);
  EXPECT_NE(ptr1, nullptr);
  EXPECT_NE(ptr2, nullptr);
  EXPECT_EQ(m_allocator->getNumAllocations(), 2);

  m_allocator->free(ptr1);
  EXPECT_EQ(m_allocator->getNumAllocations(), 1);

  void* ptr3 = m_allocator->allocate(OBJECT_SIZE, OBJECT_ALIGNMENT);
  EXPECT_EQ(ptr3, ptr1);  // Should reuse freed block
  EXPECT_EQ(m_allocator->getNumAllocations(), 2);
}

TEST_F(PoolAllocatorTest, MakeNew) {
  TestStruct* obj = m_struct_allocator->makeNew<TestStruct>(42, 777);
  EXPECT_NE(obj, nullptr);
  EXPECT_EQ(obj->a, 42);
  EXPECT_EQ(obj->b, 777);
  EXPECT_EQ(m_struct_allocator->getNumAllocations(), 1);
}
