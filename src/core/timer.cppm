module;
#include <chrono>
#include <cmath>
export module game_engine.core:timer;

namespace {
constexpr double MILLISECONDS_IN_SECOND = 1000.0;
}

namespace game_engine::core {

export class Timer {
 public:
  Timer() : m_startTime(std::chrono::high_resolution_clock::now()), m_started(true) {}

  ~Timer() = default;

  void start() {
    m_started = true;
    m_startTime = std::chrono::high_resolution_clock::now();
  }

  void stop() { m_started = false; }

  [[nodiscard]] bool isStarted() const { return m_started; }

  [[nodiscard]] uint64_t getElapsedMilliseconds() const {
    std::chrono::milliseconds time(0);

    if (m_started) {
      time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_startTime);
    }

    return time.count();
  }

  [[nodiscard]] double_t getElapsedSeconds() const { return static_cast<double>(getElapsedMilliseconds()) / MILLISECONDS_IN_SECOND; }

 private:
  std::chrono::high_resolution_clock::time_point m_startTime;
  bool m_started;
};
}  // namespace game_engine::core