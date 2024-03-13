//
// Created by marvi on 11.03.2024.
//
#pragma once

#include <cstdint>
#include <vector>

#include "Scene.h"

namespace Engine {
    class SceneManager {
        friend class Engine;

    private:
        std::vector<Scene *> m_scenes; // stack with gamestates
        std::vector<Scene *> m_inactiveScenes; // stack of gamestates which should be deleted

    public:
        SceneManager();

        ~SceneManager();

        void enterNewScene(Scene *scene);

        void leaveCurrentScene();

        void replaceCurrentScene(Scene *scene);

        Scene *getCurrentScene() const;

        void update(uint64_t deltaTime);
    };
}
