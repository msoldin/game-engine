module;
#include <SDL3/SDL.h>
#include <glog/logging.h>
#include <glm/glm.hpp>
export module game_engine.gfx.sdl:renderer;

import game_engine.error;
import game_engine.gfx.api;
import :texture;

namespace game_engine::gfx::sdl {
export class SdlRenderer final : public Renderer {
 private:
  std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> m_window;
  std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> m_renderer;

 public:
  SdlRenderer() : m_window(nullptr, SDL_DestroyWindow), m_renderer(nullptr, SDL_DestroyRenderer) {
    LOG(INFO) << "Initializing SDL_Renderer" << &std::endl;
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
      throw error::Error(error::ErrorType::kEcsError, "SDL_Init Error: '{}'", SDL_GetError());
    }

    SDL_Window* raw_window = SDL_CreateWindow("SDL3 Quad Example", 800, 600, SDL_WINDOW_VULKAN);
    if (!raw_window) {
      SDL_Quit();
      throw error::Error(error::ErrorType::kEcsError, "SDL_CreateWindow Error: '{}'", SDL_GetError());
    }
    m_window.reset(raw_window);

    SDL_Renderer* raw_renderer = SDL_CreateRenderer(m_window.get(), nullptr);
    if (!raw_renderer) {
      SDL_Quit();
      throw error::Error(error::ErrorType::kEcsError, "SDL_CreateRenderer Error: '{}'", SDL_GetError());
    }
    m_renderer.reset(raw_renderer);
  }
  ~SdlRenderer() override { SDL_Quit(); }
  void render() const override { SDL_RenderPresent(m_renderer.get()); }
  void render_rectangle(const Rectangle& rectangle, const Color& color) override {
    SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
    const SDL_FRect sdl_rect = {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
    SDL_RenderRect(m_renderer.get(), &sdl_rect);
  }
  void render_texture(const std::shared_ptr<Texture>& texture) override {
    if (auto const sdl_texture = std::dynamic_pointer_cast<SdlTexture>(texture)) {
      constexpr SDL_FRect clip = {0, 0, 0};
      constexpr SDL_FRect render_target = {0, 0, 0};
      SDL_RenderTexture(m_renderer.get(), sdl_texture->get_native_texture(), &clip, &render_target);
    }
  }
  SDL_Renderer& get_native_renderer() const { return *m_renderer.get(); }
  void clear() const;
};
}  // namespace game_engine::gfx::sdl
