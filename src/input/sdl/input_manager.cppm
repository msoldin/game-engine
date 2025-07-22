module;
#include <SDL3/SDL.h>
export module game_engine.input.sdl:input_manager;

import game_engine.input.api;

namespace game_engine::input::sdl {

export class SdlInputManager final : public InputManager {
 public:
  SdlInputManager() : m_quit_requested(false) {}
  ~SdlInputManager() override = default;

  void update() override {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          m_quit_requested = true;
          break;
        default:
          break;
      }
    }
  }

  bool quit_requested() const override { return m_quit_requested; }

 private:
  bool m_quit_requested;
};

}  // namespace game_engine::input::sdl