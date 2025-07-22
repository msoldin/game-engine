module;
#include <cstdint>
export module game_engine.core:scene;

namespace game_engine::core {
export class Scene {
 public:
  virtual ~Scene() = default;
  virtual void entered() = 0;
  virtual void leaving() = 0;
  virtual void obscuring() = 0;
  virtual void revealed() = 0;
  virtual void update(uint64_t dt) = 0;
};
}  // namespace game_engine::core
