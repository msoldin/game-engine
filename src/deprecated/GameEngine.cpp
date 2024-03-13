//
// Created by marvi on 09.03.2024.
//
#include "deprecated/GameEngine.h"

#include "deprecated/Timer.h"

#include <SDL2/SDL.h>

#include <glog/logging.h>

namespace Engine {
    Engine::Engine(): m_renderManager(new RenderManager()), m_inputManager(new InputManager()),
                      m_sceneManager(new SceneManager) {
    }

    Engine::~Engine() {
        delete m_inputManager;
        delete m_renderManager;
        delete m_sceneManager;
    }

    void Engine::run() {
        m_running = true;

        uint64_t countedFrames = 0;
        uint64_t countedUpdates = 0;
        uint64_t accumulator = 0;

        Timer *fpsTimer = new Timer();
        fpsTimer->start();

        Timer *frameTimer = new Timer();


        while (m_running) {
            frameTimer->start();
            int avgFps = countedFrames / fpsTimer->getSeconds();
            int avgUps = countedUpdates / fpsTimer->getSeconds();
            LOG(INFO) << "FPS: " << avgFps << " UPS: " << avgUps;

            while (accumulator >= SCREEN_TIME_PER_FRAME) {
                m_renderManager->clearBackBuffer();
                update(SCREEN_TIME_PER_FRAME);
                accumulator -= SCREEN_TIME_PER_FRAME;
                countedUpdates++;
            }

            m_renderManager->render();
            countedFrames++;

            uint64_t frameTime = frameTimer->getMilliseconds();
            if (frameTime < SCREEN_TIME_PER_FRAME) {
                SDL_Delay(SCREEN_TIME_PER_FRAME - frameTime);
                frameTime = SCREEN_TIME_PER_FRAME;
            }
            accumulator += frameTime;
        }
    }

    void Engine::update(uint64_t deltaTime) {
        m_inputManager->update();
        if (m_inputManager->quitRequested()) {
            m_running = false;
        }
        m_sceneManager->update(deltaTime);
    }
} // namespace Engine
