module;
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <memory>
#include <unordered_map>
export module game_engine.asset.sdl:asset_manager;

import game_engine.gfx;
import game_engine.error;
import game_engine.asset.api;

namespace game_engine::asset::sdl {
export class SdlAssetManager final : public AssetManager {
 public:
  explicit SdlAssetManager(const std::shared_ptr<gfx::Renderer>& renderer) {
    if (m_renderer = std::dynamic_pointer_cast<gfx::sdl::SdlRenderer>(renderer); m_renderer.lock() == nullptr) {
      throw error::Error(error::ErrorType::kAssetError, "Renderer is not a SdlRenderer or nullptr");
    }
  }
  std::shared_ptr<Texture> load_texture(std::string filename) override {
    if (auto const sdl_renderer = m_renderer.lock()) {
      if (const auto it = m_textures.find(filename); it != m_textures.end()) {
        return it->second;
      }
      SDL_Surface* tmpSurface = IMG_Load(filename.c_str());
      if (tmpSurface == nullptr) {
        throw error::Error(error::ErrorType::kAssetError, "Error loading texture: '{}'", filename);
      }
      auto texture = std::make_shared<gfx::sdl::SdlTexture>(SDL_CreateTextureFromSurface(&sdl_renderer->get_native_renderer(), tmpSurface));
      m_textures.insert({filename, texture});

      SDL_DestroySurface(tmpSurface);

      return texture;
    }
    throw error::Error(error::ErrorType::kAssetError, "Error loading texture: '{}', renderer is nullptr", filename);
  }

 private:
  std::weak_ptr<gfx::sdl::SdlRenderer> m_renderer;
  std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
};
}  // namespace game_engine::asset::sdl
