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
  gfx::Window m_window{WIDHT, HEIGHT, "Vulkan"};
  gfx::Device m_device{m_window};
  gfx::Renderer m_renderer{m_window, m_device};
  gfx::RenderSystem m_renderSystem{m_device, m_renderer.getSwapChainRenderPass()};
  gfx::Camera m_camera{};
  std::vector<gfx::GameObject> m_gameObjects;
  bool m_running = false;
};
} // namespace vulkan_engine::core