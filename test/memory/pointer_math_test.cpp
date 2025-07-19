#include <gtest/gtest.h>

import vulkan_engine.memory;

using vulkan_engine::memory::pointer_math::alignForward;

TEST(AlignForwardTests, BasicAlignment4) {
  void* result = alignForward(reinterpret_cast<void*>(0x00000003), 4);
  EXPECT_EQ(reinterpret_cast<size_t>(result), 0x00000004);
}

TEST(AlignForwardTests, BasicAlignment8) {
  void* result = alignForward(reinterpret_cast<void*>(0x00000005), 8);
  EXPECT_EQ(reinterpret_cast<size_t>(result), 0x00000008);
}

TEST(AlignForwardTests, Alignment1) {
  void* result = alignForward(reinterpret_cast<void*>(0x00000003), 1);
  EXPECT_EQ(reinterpret_cast<size_t>(result), 0x00000003);
}

TEST(AlignForwardTests, LargeAlignment) {
  void* result = alignForward(reinterpret_cast<void*>(0x00000005), 1024);
  EXPECT_EQ(reinterpret_cast<size_t>(result), 0x00000400);
}

TEST(AlignForwardTests, NullPointer) {
  void* result = alignForward(nullptr, 8);
  EXPECT_EQ(result, nullptr);
}

TEST(AlignForwardTests, AlignmentBehavior) {
  void* result = alignForward(reinterpret_cast<void*>(0x00000005), 4);
  EXPECT_EQ(reinterpret_cast<size_t>(result) % 4, 0);
}