//
// Created by marvi on 09.03.2024.
//
#pragma once
#include <cstdint>

#include "InputManager.h"
#include "RenderManager.h"
#include "SceneManager.h"

namespace Engine {
    class Engine {
    private:
        bool m_running = false;
        static constexpr uint64_t SCREEN_TIME_PER_FRAME = 1000 / 60;

    public:
        RenderManager *m_renderManager;
        InputManager *m_inputManager;
        SceneManager *m_sceneManager;

    public:
        Engine();

        ~Engine();

        void run();

    private:
        void update(uint64_t deltaTime);
    };
} // namespace Engine
