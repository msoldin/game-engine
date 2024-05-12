//
// Created by marvi on 27.03.2024.
//

#include "core/timer.h"

namespace vulkan_engine::core {
constexpr double MILLISECONDS_IN_SECOND = 1000.0;

Timer::Timer() : m_started(false) {
}

Timer::~Timer() = default;

void Timer::start() {
  m_started = true;
  m_startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
  m_started = false;
}

uint64_t Timer::getElapsedMilliseconds() const {
  std::chrono::milliseconds time(0);

  if (m_started) {
    time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - m_startTime);
  }

  return time.count();
}

double_t Timer::getElapsedSeconds() const {
  return static_cast<double>(getElapsedMilliseconds()) / MILLISECONDS_IN_SECOND;
}

bool Timer::isStarted() const {
  return m_started;
}
} // namespace vulkan_engine::core