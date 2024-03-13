//
// Created by marvi on 11.03.2024.
//

#pragma once

namespace Engine {
    class Scene {
    public:
        virtual ~Scene() {
        };

        virtual void entered() {
        }; //Entering the GameState for the first time
        virtual void leaving() {
        }; //Leaving the GameState forever
        virtual void obscuring() {
        }; //Gets called when another GameState obscures the current GameState
        virtual void revealed() {
        }; //Gets called when the GameState is reentered
        virtual void update(uint64_t deltaTime) const {
        }; //Everything that needs to be updated in the current GameState
    };
}
