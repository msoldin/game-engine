#include <gtest/gtest.h>

import vulkan_engine.memory;

class LinearAllocatorTest : public ::testing::Test {
 protected:
  static constexpr size_t ALLOCATOR_SIZE = 1024;  // 1 KB
  std::shared_ptr<vulkan_engine::memory::LinearAllocator> m_allocator;

  void SetUp() override { m_allocator = std::make_shared<vulkan_engine::memory::LinearAllocator>(ALLOCATOR_SIZE); }
};

TEST_F(LinearAllocatorTest, BasicAllocation) {
  void* ptr = m_allocator->allocate(16, alignof(int));
  EXPECT_NE(ptr, nullptr);  // Allocation should succeed
  EXPECT_EQ(m_allocator->getNumAllocations(), 1);
  EXPECT_EQ(m_allocator->getSpaceLeft(), ALLOCATOR_SIZE - 16);
}

TEST_F(LinearAllocatorTest, ProperAlignment) {
  void* ptr1 = m_allocator->allocate(1, 16);
  EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr1) % 16, 0); // Ensure alignment

  void* ptr2 = m_allocator->allocate(1, 32);
  EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr2) % 32, 0); // Ensure next alignment
}

TEST_F(LinearAllocatorTest, Exhaustion) {
  void* ptr = m_allocator->allocate(ALLOCATOR_SIZE, 1);
  EXPECT_NE(ptr, nullptr);  // Should succeed

  void* ptr2 = m_allocator->allocate(1, 1);
  EXPECT_EQ(ptr2, nullptr); // Should fail since memory is exhausted
  EXPECT_EQ(m_allocator->getSpaceLeft(), 0);
}

TEST_F(LinearAllocatorTest, MultipleAllocations) {
  void* ptr1 = m_allocator->allocate(32, alignof(int));
  void* ptr2 = m_allocator->allocate(64, alignof(double));
  void* ptr3 = m_allocator->allocate(128, alignof(float));

  EXPECT_NE(ptr1, nullptr);
  EXPECT_NE(ptr2, nullptr);
  EXPECT_NE(ptr3, nullptr);
  EXPECT_LT(ptr1, ptr2); // Ensure allocations are sequential
  EXPECT_LT(ptr2, ptr3);
  EXPECT_EQ(m_allocator->getNumAllocations(), 3);
}


TEST_F(LinearAllocatorTest, FreeShouldAssert) {
#ifdef NDEBUG
  GTEST_SKIP() << "Skipping free test in release mode";
#else
  EXPECT_DEATH(m_allocator->free(nullptr), "Use clear\\(\\) to deallocate memory in LinearAllocator");
#endif
}

TEST_F(LinearAllocatorTest, MakeNew) {
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