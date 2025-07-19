#include <gtest/gtest.h>

import vulkan_engine.memory;

class StackAllocatorTest : public ::testing::Test {
 protected:
  static constexpr size_t ALLOCATOR_SIZE = 1024;  // 1 KB
  static constexpr size_t ALLOCATION_HEADER_SIZE = 16;
  std::shared_ptr<vulkan_engine::memory::StackAllocator> m_allocator;

  void SetUp() override { m_allocator = std::make_shared<vulkan_engine::memory::StackAllocator>(ALLOCATOR_SIZE); }
};

TEST_F(StackAllocatorTest, BasicAllocation) {
  void* ptr = m_allocator->allocate(16, alignof(int));
  EXPECT_NE(ptr, nullptr);
  EXPECT_EQ(m_allocator->getNumAllocations(), 1);
  EXPECT_EQ(m_allocator->getSpaceLeft(), ALLOCATOR_SIZE - ALLOCATION_HEADER_SIZE - 16);
}

TEST_F(StackAllocatorTest, ProperAlignment) {
  void* ptr1 = m_allocator->allocate(1, 16);
  EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr1) % 16, 0);

  void* ptr2 = m_allocator->allocate(1, 32);
  EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr2) % 32, 0);
}

TEST_F(StackAllocatorTest, Exhaustion) {
  void* ptr = m_allocator->allocate(ALLOCATOR_SIZE - ALLOCATION_HEADER_SIZE, 1);
  EXPECT_NE(ptr, nullptr);

  void* ptr2 = m_allocator->allocate(1, 1);
  EXPECT_EQ(ptr2, nullptr);
  EXPECT_EQ(m_allocator->getSpaceLeft(), 0);
}

TEST_F(StackAllocatorTest, LifoDeallocation) {
  void* ptr1 = m_allocator->allocate(32, alignof(int));
  void* ptr2 = m_allocator->allocate(64, alignof(double));
  void* ptr3 = m_allocator->allocate(128, alignof(float));

  EXPECT_NE(ptr1, nullptr);
  EXPECT_NE(ptr2, nullptr);
  EXPECT_NE(ptr3, nullptr);
  EXPECT_EQ(m_allocator->getNumAllocations(), 3);

  m_allocator->free(ptr3);
  EXPECT_EQ(m_allocator->getNumAllocations(), 2);

  m_allocator->free(ptr2);
  EXPECT_EQ(m_allocator->getNumAllocations(), 1);

  m_allocator->free(ptr1);
  EXPECT_EQ(m_allocator->getNumAllocations(), 0);
}

TEST_F(StackAllocatorTest, InvalidDeallocationOrder) {
  void* ptr1 = m_allocator->allocate(32, alignof(int));
  void* ptr2 = m_allocator->allocate(64, alignof(double));
#ifdef NDEBUG
  GTEST_SKIP() << "Skipping assert test in release mode";
#else
  EXPECT_DEATH(m_allocator->free(ptr1), "StackAllocator deallocation must be in reverse order of allocation.");
#endif
}

TEST_F(StackAllocatorTest, MakeNew) {
  struct TestStruct {
    int a;
    float b;
    TestStruct(const int x, const float y) : a(x), b(y) {}
  };

  TestStruct* obj = m_allocator->makeNew<TestStruct>(42, 3.14f);
  EXPECT_NE(obj, nullptr);
  EXPECT_EQ(obj->a, 42);
  EXPECT_EQ(obj->b, 3.14f);
  EXPECT_EQ(m_allocator->getNumAllocations(), 1);
}