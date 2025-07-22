module;
#include <memory>
export module game_engine.asset.sdl;

import game_engine.gfx;
export import :asset_manager;

namespace game_engine::asset {
export std::shared_ptr<AssetManager> create_asset_manager(std::shared_ptr<gfx::Renderer> renderer) { return std::make_shared<sdl::SdlAssetManager>(renderer); }
}  // namespace game_engine::asset
