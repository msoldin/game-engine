//
// Created by marvi on 27.03.2024.
//

#pragma once
#include <chrono>
#include <cmath>

namespace vulkan_engine::core {
class Timer {
 public:
  Timer();
  ~Timer();
  void start();
  void stop();
  bool isStarted() const;
  uint64_t getElapsedMilliseconds() const;
  double_t getElapsedSeconds() const;

 private:
  std::chrono::high_resolution_clock::time_point start_time_;
  bool started_;
};
}  // namespace vulkan_engine::core
