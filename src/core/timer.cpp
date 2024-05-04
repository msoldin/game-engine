//
// Created by marvi on 27.03.2024.
//

#include "core/timer.h"

namespace vulkan_engine::core {
Timer::Timer() : started_(false) {}

Timer::~Timer() = default;

void Timer::start() {
  started_ = true;
  start_time_ = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
  started_ = false;
}

uint64_t Timer::getElapsedMilliseconds() const {
  std::chrono::milliseconds time(0);

  if (started_) {
    time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start_time_);
  }

  return time.count();
}

double_t Timer::getElapsedSeconds() const {
  return static_cast<double>(getElapsedMilliseconds()) / 1000.0;
}

bool Timer::isStarted() const {
  return started_;
}
}  // namespace vulkan_engine::core