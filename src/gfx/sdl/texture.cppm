module;
#include <SDL3/SDL.h>
export module game_engine.gfx.sdl:texture;

import game_engine.gfx.api;

namespace game_engine::gfx::sdl {

export class SdlTexture : public Texture {
 public:
  explicit SdlTexture(SDL_Texture* m_texture) : m_texture(m_texture) {}
  ~SdlTexture() override { SDL_DestroyTexture(m_texture); }
  SDL_Texture* get_native_texture() const { return m_texture; }
  int width() const override { return 0; }
  int height() const override { return 0; }

 private:
  SDL_Texture* m_texture;
};
}  // namespace game_engine::gfx::sdl