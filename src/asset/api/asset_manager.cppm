module;
#include <memory>
export module game_engine.asset.api:asset_manager;

import game_engine.gfx;

namespace game_engine::asset {
export class AssetManager {
 public:
  virtual ~AssetManager() = default;
  virtual std::shared_ptr<Texture> load_texture(const std::string& filename) = 0;
  virtual std::shared_ptr<Texture> load_texture(const std::string& filename, float offset_x, float offset_y) = 0;
  virtual std::shared_ptr<Animation> load_animation(const std::string& filename) = 0;
  virtual std::shared_ptr<Animation> load_animation(const std::string& filename, float offset_x, float offset_y) = 0;
};
}  // namespace game_engine::asset
