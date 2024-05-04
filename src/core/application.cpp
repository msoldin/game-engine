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
  running_ = true;

  uint64_t countedFrames = 0;
  uint64_t countedUpdates = 0;
  uint64_t accumulator = 0;

  Timer fpsTimer{};
  fpsTimer.start();

  Timer frameTimer{};

  while (!window_.shouldClose()) {
    glfwPollEvents();

    frameTimer.start();
    double_t avgFps =
        static_cast<double_t>(countedFrames) / fpsTimer.getElapsedSeconds();
    double_t avgUps =
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

  vkDeviceWaitIdle(device_.device());
}

void Application::update(uint64_t deltaTime) {
  float aspect = renderer_.getAspectRatio();
  camera_.setViewDirection(glm::vec3(0.f), glm::vec3(0.0f, 0.f, 1.f));
  camera_.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
  if (auto command_buffer = renderer_.beginFrame()) {
    renderer_.beginSwapChainRenderPass(command_buffer);
    render_system_.renderGameObjects(command_buffer, game_objects_, camera_);
    renderer_.endSwapChainRenderPass(command_buffer);
    renderer_.endFrame();
  }
}

void Application::loadGameObjects() {
  std::shared_ptr<gfx::Model> model = createCubeModel(device_, {0.f, 0.f, 0.f});

  auto cube = gfx::GameObject::createGameObject();
  cube.model = model;
  cube.color = {.1f, .8f, .1f};
  cube.transform.translation = {.0f, .0f, 2.5f};
  cube.transform.scale = {.5f, .5f, .5f};

  game_objects_.push_back(std::move(cube));
}
}  // namespace vulkan_engine::core
