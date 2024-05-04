//
// Created by marvi on 27.03.2024.
//

#pragma once
#include <cstdint>

#include "render_system.h"
#include "renderer.h"

namespace vulkan_engine::core {

class Application {

 public:
  Application();
  ~Application() = default;
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;

  void run();

 private:
  void update(uint64_t deltaTime);
  void loadGameObjects();

  static constexpr uint64_t SCREEN_TIME_PER_FRAME = 1000 / 60;
  static constexpr int WIDHT = 800;
  static constexpr int HEIGHT = 600;
  gfx::Window window_{WIDHT, HEIGHT, "Vulkan"};
  gfx::Device device_{window_};
  gfx::Renderer renderer_{window_, device_};
  gfx::RenderSystem render_system_{device_, renderer_.getSwapChainRenderPass()};
  gfx::Camera camera_{};
  std::vector<gfx::GameObject> game_objects_;
  bool running_ = false;
};

}  // namespace vulkan_engine::core
