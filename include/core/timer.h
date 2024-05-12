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

  [[nodiscard]] bool isStarted() const;

  [[nodiscard]] uint64_t getElapsedMilliseconds() const;

  [[nodiscard]] double_t getElapsedSeconds() const;

private:
  std::chrono::high_resolution_clock::time_point m_startTime;
  bool m_started;
};
} // namespace vulkan_engine::core