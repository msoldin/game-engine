module;
#include <memory>
export module game_engine.gfx.api:renderer;

import :color;
import :rectangle;
import :texture;


namespace game_engine::gfx {
export class Renderer {
 public:
  virtual ~Renderer() = default;
  virtual void render() const = 0;
  virtual void render_rectangle(const Rectangle& rectangle, const Color& color) = 0;
  virtual void render_texture(const std::shared_ptr<Texture>& texture) = 0;
};
}  // namespace game_engine::gfx