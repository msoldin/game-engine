module;
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <memory>
#include <unordered_map>
#include <vector>

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
  std::shared_ptr<Texture> load_texture(const std::string& filename) override { return load_texture(filename, 0.0f, 0.0f); }
  std::shared_ptr<Texture> load_texture(const std::string& filename, float offset_x, float offset_y) override {
    if (auto const sdl_renderer = m_renderer.lock()) {
      if (const auto it = m_textures.find(filename); it != m_textures.end()) {
        return it->second;
      }
      SDL_Surface* tmpSurface = IMG_Load(filename.c_str());
      if (tmpSurface == nullptr) {
        throw error::Error(error::ErrorType::kAssetError, "Error loading texture: '{}'", filename);
      }
      auto texture = std::make_shared<gfx::sdl::SdlTexture>(SDL_CreateTextureFromSurface(&sdl_renderer->get_native_renderer(), tmpSurface), offset_x, offset_y);
      m_textures.insert({filename, texture});

      SDL_DestroySurface(tmpSurface);

      return texture;
    }
    throw error::Error(error::ErrorType::kAssetError, "Error loading texture: '{}', renderer is nullptr", filename);
  }
  std::shared_ptr<Animation> load_animation(const std::string& filename) override { return load_animation(filename, 0.0f, 0.0f); }
  std::shared_ptr<Animation> load_animation(const std::string& filename, float offset_x, float offset_y) override {
    if (auto const sdl_renderer = m_renderer.lock()) {
      if (const auto it = m_animations.find(filename); it != m_animations.end()) {
        return it->second;
      }

      IMG_Animation* sdl_animation = IMG_LoadAnimation(filename.c_str());
      if (sdl_animation == nullptr) {
        throw error::Error(error::ErrorType::kAssetError, "Error loading animation: '{}'", filename);
      }

      std::vector<SDL_Texture*> textures;
      std::vector<int> delays;

      for (int i = 0; i < sdl_animation->count; ++i) {
        textures.push_back(SDL_CreateTextureFromSurface(&sdl_renderer->get_native_renderer(), sdl_animation->frames[i]));
        delays.push_back(sdl_animation->delays[i]);
      }

      auto animation = std::make_shared<gfx::sdl::SdlAnimation>(textures, delays, offset_x, offset_y);
      m_animations.insert({filename, animation});

      IMG_FreeAnimation(sdl_animation);

      return animation;
    }
    throw error::Error(error::ErrorType::kAssetError, "Error loading animation: '{}', renderer is nullptr", filename);
  }

 private:
  std::weak_ptr<gfx::sdl::SdlRenderer> m_renderer;
  std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
  std::unordered_map<std::string, std::shared_ptr<Animation>> m_animations;
};
}  // namespace game_engine::asset::sdl
