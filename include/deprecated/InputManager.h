#pragma once

#include "InputDefinitions.h"

namespace Engine {
    class InputManager {
        friend class Engine;

    private:
        const uint8_t *m_keyboard;

        uint32_t m_mouse;

        int m_mouseX;

        int m_mouseY;

        bool m_keyDown[KEYBOARD_SIZE];

        bool m_keyUp[KEYBOARD_SIZE];

        bool m_mouseDown[MOUSE_MAX];

        bool m_mouseUp[MOUSE_MAX];

        bool m_willQuit;

        bool m_isLocked;

    public:
        void update();

        bool isKeyDown(int key) const;

        bool isKeyUp(int key) const;

        bool shift() const;

        bool ctrl() const;

        bool alt() const;

        bool isMouseDown(MouseButton button) const;

        bool isMouseUp(MouseButton button) const;

        bool isKeyPressed(KeyboardKey key) const;

        bool isMousePressed(MouseButton button) const;

        bool quitRequested() const;

        int getMouseX() const;

        int getMouseY() const;

        void lock();

        void unlock();

    private:
        InputManager();

        ~InputManager();
    };
}
