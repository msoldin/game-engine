module;
#include <memory>
export module game_engine.input.sdl;

export import :input_manager;

namespace game_engine::input {
export std::shared_ptr<InputManager> create_input_manager() { return std::make_shared<sdl::SdlInputManager>(); }
}  // namespace game_engine::input