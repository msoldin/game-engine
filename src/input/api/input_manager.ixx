module;
export module game_engine.input.api:input_manager;

namespace game_engine::input {
export class InputManager {
 public:
  virtual ~InputManager() = default;
  virtual void update() = 0;
  virtual bool quit_requested() const = 0;
};
}  // namespace game_engine::input
