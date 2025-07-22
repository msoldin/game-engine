module;
#include <memory>
export module game_engine.gfx.sdl;

export import :renderer;
export import :texture;

namespace game_engine::gfx {
export std::shared_ptr<Renderer> create_renderer() { return std::make_shared<sdl::SdlRenderer>(); }
}  // namespace game_engine::gfx
