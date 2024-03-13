#include "deprecated/SceneManager.h"

namespace Engine {
    SceneManager::SceneManager() : m_scenes(std::vector<Scene *>()),
                                   m_inactiveScenes(std::vector<Scene *>()) {
    }

    SceneManager::~SceneManager() {
        bool first = true;

        //Iterating through Scenes and deleting them while
        //calling the revealed- and leaving-functions
        for (Scene *currentScene: m_scenes) {
            if (!first) {
                currentScene->revealed();
            }

            currentScene->leaving();
            delete currentScene;
            first = false;
        }
        m_scenes.clear();

        for (auto const &currentScene: m_inactiveScenes) {
            delete currentScene;
        }

        m_inactiveScenes.clear();
    }

    void SceneManager::leaveCurrentScene() {
        // leaves the Scene which is on top of the stack
        if (!m_scenes.empty()) {
            Scene *currentScene = this->m_scenes.back();
            currentScene->leaving();
            m_scenes.pop_back();

            m_inactiveScenes.push_back(currentScene); //delete

            if (!m_scenes.empty()) {
                m_scenes.back()->revealed();
            }
        }
    }

    void SceneManager::replaceCurrentScene(Scene *Scene) {
        leaveCurrentScene();
        enterNewScene(Scene);
    }

    void SceneManager::enterNewScene(Scene *newScene) {
        // pushes a new Scene on the stack
        if (!m_scenes.empty()) {
            m_scenes.back()->obscuring();
        }

        m_scenes.push_back(newScene);
        m_scenes.back()->entered();
    }

    Scene *SceneManager::getCurrentScene() const {
        if (!m_scenes.empty()) {
            return m_scenes.back();
        }
        return nullptr;
    }

    void SceneManager::update(uint64_t deltaTime) {
        //Clear inactive Scenes
        for (const auto &toDelete: m_inactiveScenes) {
            delete toDelete;
        }
        m_inactiveScenes.clear();

        if (!m_scenes.empty()) {
            m_scenes.back()->update(deltaTime);
        }
    }
}
