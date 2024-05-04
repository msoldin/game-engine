//
// Created by marvi on 03.05.2024.
//

#ifndef COORDINATOR_H
#define COORDINATOR_H
#include <memory>

#include "entity_manager.h"

namespace vulkan_engine::ecs {
    class Coordinator final {
    private:
        std::unique_ptr<EntityManager> m_entityManager;
    };
}

#endif //COORDINATOR_H
