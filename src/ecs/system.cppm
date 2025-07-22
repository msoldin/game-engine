module;
#include <cstdint>
export module game_engine.ecs:system;

import game_engine.memory;
import :entity_manager;
import :component_manager;

namespace game_engine::ecs {

export class System {
 public:
  virtual ~System() = default;

  virtual void preUpdate(uint64_t dt) = 0;

  virtual void update(uint64_t dt) = 0;

  virtual void postUpdate(uint64_t dt) = 0;
};
}  // namespace game_engine::ecs