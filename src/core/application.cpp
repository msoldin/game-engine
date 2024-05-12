//
// Created by marvi on 27.03.2024.
//
#include <glog/logging.h>
#include <thread>

#include <core/application.h>
#include <core/timer.h>

#include "render_system.h"

namespace vulkan_engine::core {
std::unique_ptr<gfx::Model> createCubeModel(gfx::Device& device,
                                            glm::vec3 offset) {
  std::vector<gfx::Model::Vertex> vertices{

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
  for (auto& v : vertices) {
    v.m_position += offset;
  }
  return std::make_unique<gfx::Model>(device, vertices);
}

Application::Application() {
  loadGameObjects();
}

void Application::run() {
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
    const double_t avgFps =
        static_cast<double_t>(countedFrames) / fpsTimer.getElapsedSeconds();
    const double_t avgUps =
        static_cast<double_t>(countedUpdates) / fpsTimer.getElapsedSeconds();
    LOG(INFO) << "FPS: " << avgFps << " UPS: " << avgUps;

    while (accumulator >= SCREEN_TIME_PER_FRAME) {
      update(SCREEN_TIME_PER_FRAME);
      accumulator -= SCREEN_TIME_PER_FRAME;
      countedUpdates++;
    }

    //m_renderManager->render();
    countedFrames++;

    uint64_t frameTime = frameTimer.getElapsedMilliseconds();
    if (frameTime < SCREEN_TIME_PER_FRAME) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(SCREEN_TIME_PER_FRAME - frameTime));
      frameTime = SCREEN_TIME_PER_FRAME;
    }
    accumulator += frameTime;
  }

  vkDeviceWaitIdle(m_device.device());
}

void Application::update(uint64_t deltaTime) {
  float aspect = m_renderer.getAspectRatio();
  m_camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.0f, 0.f, 1.f));
  m_camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
  if (auto *command_buffer = m_renderer.beginFrame()) {
    m_renderer.beginSwapChainRenderPass(command_buffer);
    m_renderSystem.renderGameObjects(command_buffer, m_gameObjects, m_camera);
    m_renderer.endSwapChainRenderPass(command_buffer);
    m_renderer.endFrame();
  }
}

void Application::loadGameObjects() {
  std::shared_ptr<gfx::Model> model = createCubeModel(m_device, {0.f, 0.f, 0.f});

  auto cube = gfx::GameObject::createGameObject();
  cube.model = model;
  cube.color = {.1f, .8f, .1f};
  cube.transform.translation = {.0f, .0f, 2.5f};
  cube.transform.scale = {.5f, .5f, .5f};

  m_gameObjects.push_back(std::move(cube));
}
} // namespace vulkan_engine::core