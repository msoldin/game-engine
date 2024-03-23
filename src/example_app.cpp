//
// Created by marvi on 14.03.2024.
//

#include "example_app.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "render_system.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace vulkan_engine::gfx {
ExampleApp::ExampleApp() { loadGameObjects(); }

ExampleApp::~ExampleApp() {}

void ExampleApp::run() {
  RenderSystem render_system(device_, renderer_.getSwapChainRenderPass());

  while (!window_.shouldClose()) {
    glfwPollEvents();
    if (auto command_buffer = renderer_.beginFrame()) {
      renderer_.beginSwapChainRenderPass(command_buffer);
      render_system.renderGameObjects(command_buffer, game_objects_);
      renderer_.endSwapChainRenderPass(command_buffer);
      renderer_.endFrame();
    }
  }
}

void ExampleApp::loadGameObjects() {
  std::vector<Model::Vertex> vertices{
      {{0.0f, -0.5f}, {1, 0, 0}},
      {{0.5f, 0.5f}, {0, 1, 0}},
      {{-0.5f, 0.5f}, {0, 0, 1}},
  };
  auto m_model = std::make_shared<Model>(device_, vertices);

  auto triangle = GameObject::createGameObject();
  triangle.model = m_model;
  triangle.color = {.1f, .8f, .1f};
  triangle.transform.translation = {0.2f, 0.0f};
  triangle.transform.scale = {2.f, .5f};
  triangle.transform.rotation = glm::two_pi<float>() * 0.25f;

  game_objects_.push_back(std::move(triangle));
}

} // namespace vulkan_engine::gfx
