module;
#include <glog/logging.h>

#include <cmath>
#include <thread>
export module game_engine.core:application;

import game_engine.gfx;
import game_engine.asset;
import game_engine.input;
import :timer;
import :scene_manager;

namespace game_engine::core {
export class Application final {
 public:
  Application() {
    m_renderer = gfx::create_renderer();
    m_scene_manager = core::create_scene_manager();
    m_input_manager = input::create_input_manager();
    m_asset_manager = asset::create_asset_manager(m_renderer);
  }
  ~Application() {}

  [[noreturn]] void run() {
    m_running = true;

    uint64_t countedFrames = 0;
    uint64_t countedUpdates = 0;
    uint64_t accumulator = 0;

    Timer fpsTimer{};
    fpsTimer.start();

    Timer frameTimer{};

    while (m_running) {
      m_input_manager->update();
      if (m_input_manager->quit_requested()) {
        m_running = false;
      }
      frameTimer.start();
      const double_t avgFps = static_cast<double_t>(countedFrames) / fpsTimer.getElapsedSeconds();
      const double_t avgUps = static_cast<double_t>(countedUpdates) / fpsTimer.getElapsedSeconds();
      LOG(INFO) << "FPS: " << avgFps << " UPS: " << avgUps;

      while (accumulator >= SCREEN_TIME_PER_FRAME) {
        m_renderer->clear();
        update(SCREEN_TIME_PER_FRAME);
        accumulator -= SCREEN_TIME_PER_FRAME;
        countedUpdates++;
      }

      countedFrames++;
      m_renderer->render();

      uint64_t frameTime = frameTimer.getElapsedMilliseconds();
      if (frameTime < SCREEN_TIME_PER_FRAME) {
        std::this_thread::sleep_for(std::chrono::milliseconds(SCREEN_TIME_PER_FRAME - frameTime));
        frameTime = SCREEN_TIME_PER_FRAME;
      }
      accumulator += frameTime;
    }
  }

  void stop() { m_running = false; }

  gfx::Renderer& renderer() const { return *m_renderer; }
  core::SceneManager& scene_manager() const { return *m_scene_manager; }
  input::InputManager& input_manager() const { return *m_input_manager; }
  asset::AssetManager& asset_manager() const { return *m_asset_manager; }

 private:
  void update(uint64_t const dt) const { m_scene_manager->update(dt); }
  static constexpr uint64_t SCREEN_TIME_PER_FRAME = 1000 / 120;
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;
  bool m_running = false;
  std::shared_ptr<gfx::Renderer> m_renderer;
  std::shared_ptr<core::SceneManager> m_scene_manager;
  std::shared_ptr<input::InputManager> m_input_manager;
  std::shared_ptr<asset::AssetManager> m_asset_manager;
};
}  // namespace game_engine::core