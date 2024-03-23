//
// Created by marvi on 16.03.2024.
//

#pragma once
#include <memory>

#include "model.h"

namespace vulkan_engine::gfx {
    struct Transform2DComponent {
        glm::vec2 translation{};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation;

        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{{c, s}, {-s, c}};
            glm::mat2 scaleMatrix = glm::mat2(glm::vec2(scale.x, 0.0f), glm::vec2(0.0f, scale.y));
            return rotMatrix * scaleMatrix;
        }
    };

    class GameObject {
    public :
        using id_t = unsigned int;

        static GameObject createGameObject() {
            static id_t currentId = 0;
            return GameObject(currentId++);
        }

        id_t getId() const {
            return m_id;
        }

        GameObject(const GameObject &) = delete;

        GameObject &operator =(const GameObject &) = delete;

        GameObject(GameObject &&) = default;

        GameObject &operator=(GameObject &&) = default;

        std::shared_ptr<Model> model{};
        glm::vec3 color{};
        Transform2DComponent transform{};

    private:
        GameObject(id_t id) : m_id(id) {
        }

        id_t m_id;
    };
}
