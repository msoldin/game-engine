#include "InputManager.h"
#include <iostream>

namespace Engine {
    InputManager::InputManager(): m_keyboard(nullptr),
                                  m_mouse(0),
                                  m_mouseX(0),
                                  m_mouseY(0),
                                  m_keyDown{false},
                                  m_keyUp{false},
                                  m_mouseDown{false},
                                  m_mouseUp{false},
                                  m_willQuit(false),
                                  m_isLocked(false) {
    }

    InputManager::~InputManager() = default;

    void InputManager::update() {
        for (int i = 0; i < KEYBOARD_SIZE; i++) {
            this->m_keyDown[i] = false;
            this->m_keyUp[i] = false;
        }
        for (int i = 0; i < MOUSE_MAX; i++) {
            this->m_mouseDown[i] = false;
            this->m_mouseUp[i] = false;
        }

        // Get key events from the OS
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    this->m_willQuit = true;
                    break;

                case SDL_KEYDOWN: {
                    this->m_keyboard = SDL_GetKeyboardState(nullptr);

                    const int index = event.key.keysym.scancode;

                    this->m_keyDown[index] = true;
                }
                break;

                case SDL_KEYUP: {
                    this->m_keyboard = SDL_GetKeyboardState(nullptr);

                    const int index = event.key.keysym.scancode;
                    this->m_keyUp[index] = true;
                }
                break;

                case SDL_MOUSEMOTION:
                    this->m_mouseX = event.motion.x;
                    this->m_mouseY = event.motion.y;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    this->m_mouse = SDL_GetMouseState(&(this->m_mouseX),
                                                      &(this->m_mouseY));

                    if (event.button.button == SDL_BUTTON_LEFT)
                        this->m_mouseDown[MOUSE_LEFT] = true;

                    else if (event.button.button == SDL_BUTTON_RIGHT)
                        this->m_mouseDown[MOUSE_RIGHT] = true;
                    break;

                case SDL_MOUSEBUTTONUP:
                    this->m_mouse = SDL_GetMouseState(&(this->m_mouseX),
                                                      &(this->m_mouseY));

                    if (event.button.button == SDL_BUTTON_LEFT)
                        this->m_mouseUp[MOUSE_LEFT] = true;

                    else if (event.button.button == SDL_BUTTON_RIGHT)
                        this->m_mouseUp[MOUSE_RIGHT] = true;
                    break;

                default:
                    break;
            }
        }
    }

    bool InputManager::isKeyDown(int key) const {
        if (this->m_isLocked) return false;

        if (key < 0 || key >= KEYBOARD_SIZE)
            return false;

        return (this->m_keyDown[key]);
    }

    bool InputManager::isKeyUp(int key) const {
        if (this->m_isLocked) return false;

        if (key < 0 || key >= KEYBOARD_SIZE)
            return false;

        return (this->m_keyUp[key]);
    }

    bool InputManager::isKeyPressed(KeyboardKey key) const {
        if (this->m_isLocked) return false;

        if (!(this->m_keyboard))
            return false;

        const int sdl_key = static_cast<int>(key);

        if (this->m_keyboard[sdl_key])
            return true;

        return false;
    }

    bool InputManager::shift() const {
        return (this->isKeyPressed(KEY_LEFT_SHIFT) ||
                this->isKeyPressed(KEY_RIGHT_SHIFT));
    }

    bool InputManager::ctrl() const {
        return (this->isKeyPressed(KEY_LEFT_CTRL) ||
                this->isKeyPressed(KEY_RIGHT_CTRL));
    }

    bool InputManager::alt() const {
        return (this->isKeyPressed(KEY_LEFT_ALT) ||
                this->isKeyPressed(KEY_RIGHT_ALT));
    }

    bool InputManager::isMouseDown(MouseButton button) const {
        if (this->m_isLocked) return false;

        if (button == MOUSE_MAX)
            return false;

        return this->m_mouseDown[button];
    }

    bool InputManager::isMouseUp(MouseButton button) const {
        if (this->m_isLocked) return false;

        if (button == MOUSE_MAX)
            return false;

        return this->m_mouseUp[button];
    }

    bool InputManager::isMousePressed(MouseButton button) const {
        if (this->m_isLocked) return false;

        switch (button) {
            case MOUSE_LEFT:
                if (this->m_mouse & SDL_BUTTON(1))
                    return true;
                break;

            case MOUSE_RIGHT:
                if (this->m_mouse & SDL_BUTTON(3))
                    return true;
                break;

            default:
                break;
        }

        return false;
    }

    int InputManager::getMouseX() const {
        return this->m_mouseX;
    }

    int InputManager::getMouseY() const {
        return this->m_mouseY;
    }

    bool InputManager::quitRequested() const {
        return (this->m_willQuit);
    }

    void InputManager::lock() {
        this->m_isLocked = true;
    }

    void InputManager::unlock() {
        this->m_isLocked = false;
    }
}
