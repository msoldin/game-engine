module;
#include <SDL3/SDL.h>

#include <vector>
export module game_engine.gfx.sdl:texture;

import game_engine.gfx.api;

namespace game_engine::gfx::sdl {

export class SdlTexture final : public Texture {
 public:
  explicit SdlTexture(SDL_Texture* texture, const float offset_x, const float offset_y) : m_offset_x(offset_x), m_offset_y(offset_y), m_texture(texture) {}
  ~SdlTexture() override { SDL_DestroyTexture(m_texture); }
  SDL_Texture* get_native_texture() const { return m_texture; }
  float width() const override { return m_texture->w; }
  float height() const override { return m_texture->h; }
  float offset_x() const override { return m_offset_x; }
  float offset_y() const override { return m_offset_y; }

 private:
  float m_offset_x;
  float m_offset_y;
  SDL_Texture* m_texture;
};

export class SdlAnimation final : public Animation {
 public:
  explicit SdlAnimation(const std::vector<SDL_Texture*>& textures, const std::vector<int>& frame_delay, const float offset_x, const float offset_y)
      : m_offset_x(offset_x), m_offset_y(offset_y), m_current_frame(0), m_time_accumulator(0), m_textures(textures), m_frame_delay(frame_delay) {}
  ~SdlAnimation() override {
    for (const auto texture : m_textures) {
      SDL_DestroyTexture(texture);
    }
  }
  SDL_Texture* get_current_frame() const { return m_textures[m_current_frame]; }
  float width() const override { return m_textures[m_current_frame]->w; }
  float height() const override { return m_textures[m_current_frame]->h; }
  float offset_x() const override { return m_offset_x; }
  float offset_y() const override { return m_offset_y; }
  void update(const uint64_t dt) override {
    if (m_textures.empty() || m_frame_delay.empty()) return;
    m_time_accumulator += dt;
    if (m_time_accumulator >= m_frame_delay[m_current_frame]) {
      m_current_frame = (m_current_frame + 1) % m_textures.size();
      m_time_accumulator = 0;
    }
  }

 private:
  float m_offset_x;
  float m_offset_y;
  int m_current_frame;
  uint64_t m_time_accumulator;
  std::vector<SDL_Texture*> m_textures;
  std::vector<int> m_frame_delay;
};

}  // namespace game_engine::gfx::sdl