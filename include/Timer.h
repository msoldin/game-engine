#pragma once

#include <SDL_stdinc.h>

namespace Engine {
    class Timer {
    private:
        uint64_t m_startTicks;
        uint64_t m_pausedTicks;
        bool m_paused;
        bool m_started;

    public:
        Timer();

        ~Timer();

        void start();

        void stop();

        void pause();

        void unpause();

        uint64_t getMilliseconds() const;

        double_t getSeconds() const;

        bool isStarted() const;

        bool isPaused() const;
    };
}
