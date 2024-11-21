//
// Created by marvi on 27.03.2024.
//

module;

#include <glog/logging.h>

#include <cstdint>
#include <thread>

#include "render_system.h"
#include "renderer.h"

export module vulkan_engine.core:application;

import :timer;

namespace {
std::unique_ptr<vulkan_engine::gfx::Model> createCubeModel(vulkan_engine::gfx::Device& device, glm::vec3 offset) {
  std::vector<vulkan_engine::gfx::Model::Vertex> vertices{

      // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

  };
  for (auto& [m_position, _] : vertices) {
    m_position += offset;
  }
  return std::make_unique<vulkan_engine::gfx::Model>(device, vertices);
}
}  // namespace

namespace vulkan_engine::core {
export class Application {
 public:
  Application() { loadGameObjects(); }

  ~Application() = default;

  Application(const Application&) = delete;

  Application& operator=(const Application&) = delete;

  void run() {
    m_running = true;

    uint64_t countedFrames = 0;
    uint64_t countedUpdates = 0;
    uint64_t accumulator = 0;

    Timer fpsTimer{};
    fpsTimer.start();

    Timer frameTimer{};

    while (!m_window.shouldClose()) {
      glfwPollEvents();

      frameTimer.start();
      const double_t avgFps = static_cast<double_t>(countedFrames) / fpsTimer.getElapsedSeconds();
      const double_t avgUps = static_cast<double_t>(countedUpdates) / fpsTimer.getElapsedSeconds();
      LOG(INFO) << "FPS: " << avgFps << " UPS: " << avgUps;

      while (accumulator >= SCREEN_TIME_PER_FRAME) {
        update(SCREEN_TIME_PER_FRAME);
        accumulator -= SCREEN_TIME_PER_FRAME;
        countedUpdates++;
      }

      // m_renderManager->render();
      countedFrames++;

      uint64_t frameTime = frameTimer.getElapsedMilliseconds();
      if (frameTime < SCREEN_TIME_PER_FRAME) {
        std::this_thread::sleep_for(std::chrono::milliseconds(SCREEN_TIME_PER_FRAME - frameTime));
        frameTime = SCREEN_TIME_PER_FRAME;
      }
      accumulator += frameTime;
    }

    vkDeviceWaitIdle(m_device.device());
  }

 private:
  void update(uint64_t deltaTime) {
    const float aspect = m_renderer.getAspectRatio();
    m_camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.0f, 0.f, 1.f));
    m_camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
    if (auto* command_buffer = m_renderer.beginFrame()) {
      m_renderer.beginSwapChainRenderPass(command_buffer);
      m_renderSystem.renderGameObjects(command_buffer, m_gameObjects, m_camera);
      m_renderer.endSwapChainRenderPass(command_buffer);
      m_renderer.endFrame();
    }
  }

  void loadGameObjects() {
    const std::shared_ptr model = createCubeModel(m_device, {0.f, 0.f, 0.f});

    auto cube = gfx::GameObject::createGameObject();
    cube.model = model;
    cube.color = {.1f, .8f, .1f};
    cube.transform.translation = {.0f, .0f, 2.5f};
    cube.transform.scale = {.5f, .5f, .5f};

    m_gameObjects.push_back(std::move(cube));
  }

  static constexpr uint64_t SCREEN_TIME_PER_FRAME = 1000 / 60;
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;
  gfx::Window m_window{WIDTH, HEIGHT, "Vulkan"};
  gfx::Device m_device{m_window};
  gfx::Renderer m_renderer{m_window, m_device};
  gfx::RenderSystem m_renderSystem{m_device, m_renderer.getSwapChainRenderPass()};
  gfx::Camera m_camera{};
  std::vector<gfx::GameObject> m_gameObjects;
  bool m_running = false;
};
}  // namespace vulkan_engine::core