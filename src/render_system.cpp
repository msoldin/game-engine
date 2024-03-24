//
// Created by marvi on 23.03.2024.
//
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "render_system.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace vulkan_engine::gfx {
struct SimplePushConstantData {
  glm::mat4 transform{1.0f};
  alignas(16) glm::vec3 color;
};

RenderSystem::RenderSystem(Device& device, VkRenderPass render_pass)
    : device_(device) {
  createPipelineLayout();
  createPipeline(render_pass);
}

RenderSystem::~RenderSystem() {
  vkDestroyPipelineLayout(device_.device(), pipeline_layout_, nullptr);
}

void RenderSystem::createPipelineLayout() {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutInfo, nullptr,
                             &pipeline_layout_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipeline layout");
  }
}

void RenderSystem::createPipeline(VkRenderPass render_pass) {
  assert(pipeline_layout_ != nullptr &&
         "Cannot create pipeline before pipeline layout");
  PipelineConfigInfo pipelineConfigInfo{};
  Pipeline::defaultPipelineConfigInfo(pipelineConfigInfo);
  pipelineConfigInfo.renderPass = render_pass;
  pipelineConfigInfo.pipelineLayout = pipeline_layout_;
  pipeline_ = std::make_unique<Pipeline>(
      device_, "..//..//vulkan-engine/shaders/simple_shader.vert.spv",
      "..//..//vulkan-engine/shaders/simple_shader.frag.spv",
      pipelineConfigInfo);
}

void RenderSystem::renderGameObjects(VkCommandBuffer command_buffer,
                                     std::vector<GameObject>& game_objects) {
  pipeline_->bind(command_buffer);
  for (auto& obj : game_objects) {
    obj.transform.rotation.y =
        glm::mod(obj.transform.rotation.y + 0.0001f, glm::two_pi<float>());
    obj.transform.rotation.x =
        glm::mod(obj.transform.rotation.x + 0.0001f, glm::two_pi<float>());

    SimplePushConstantData push{};
    push.color = obj.color;
    push.transform = obj.transform.mat4();
    vkCmdPushConstants(
        command_buffer, pipeline_layout_,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
        sizeof(SimplePushConstantData), &push);
    obj.model->bind(command_buffer);
    obj.model->draw(command_buffer);
  }
}

}  // namespace vulkan_engine::gfx
