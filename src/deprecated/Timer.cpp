#include "deprecated/Timer.h"
#include <SDL.h>

namespace Engine {
    Timer::Timer()
        : m_startTicks(0), m_pausedTicks(0), m_paused(false), m_started(false) {
    }

    Timer::~Timer() = default;

    void Timer::start() {
        // start the timer
        m_started = true;

        // Unpause timer
        m_paused = false;

        m_startTicks = SDL_GetTicks64();
        m_pausedTicks = 0;
    }

    void Timer::stop() {
        // Stop the timer
        m_started = false;

        // Unpause the timer
        m_paused = false;

        // Clear Tick variables
        m_startTicks = 0;
        m_pausedTicks = 0;
    }

    void Timer::pause() {
        if (m_started && !m_paused) {
            // Pause the timer
            m_paused = true;

            // Calculate paused ticks
            m_pausedTicks = SDL_GetTicks() - m_startTicks;
            m_startTicks = 0;
        }
    }

    void Timer::unpause() {
        if (m_started && m_paused) {
            // Unpause timer
            m_paused = false;

            // Reset the starting ticks
            m_startTicks = SDL_GetTicks64() - m_pausedTicks;

            // Reset the paused ticks
            m_pausedTicks = 0;
        }
    }

    uint64_t Timer::getMilliseconds() const {
        // The actual timer
        uint64_t time = 0;

        // If the timer is running
        if (m_started) {
            // If the timer is paused
            if (m_paused) {
                // Return the number of ticks when the timer was paused
                time = m_pausedTicks;
            } else {
                // Return the current time minus start time
                time = SDL_GetTicks64() - m_startTicks;
            }
        }

        return time;
    }

    double_t Timer::getSeconds() const { return getMilliseconds() / 1000.0; }

    bool Timer::isStarted() const { return m_started; }

    bool Timer::isPaused() const { return m_paused && m_started; }
} // namespace Engine
