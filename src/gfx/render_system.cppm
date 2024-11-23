module;
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <memory>
#include <vector>

export module vulkan_engine.gfx:render_system;

import :device;
import :camera;
import :pipeline;
import :game_object;

namespace vulkan_engine::gfx {

struct SimplePushConstantData {
  glm::mat4 transform{1.0f};
  alignas(16) glm::vec3 color;
};

export class RenderSystem {
 public:
  RenderSystem(Device& device, VkRenderPass render_pass) : device_(device) {
    createPipelineLayout();
    createPipeline(render_pass);
  }
  ~RenderSystem() { vkDestroyPipelineLayout(device_.device(), pipeline_layout_, nullptr); }
  RenderSystem(const RenderSystem&) = delete;
  RenderSystem& operator=(const RenderSystem&) = delete;

  void renderGameObjects(VkCommandBuffer command_buffer, std::vector<GameObject>& game_objects, const Camera& camera) {
    pipeline_->bind(command_buffer);

    auto projectionView = camera.getProjection() * camera.getView();

    for (auto& obj : game_objects) {
      obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
      obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.01f, glm::two_pi<float>());

      SimplePushConstantData push{};
      push.color = obj.color;
      push.transform = projectionView * obj.transform.mat4();
      vkCmdPushConstants(command_buffer, pipeline_layout_, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
      obj.model->bind(command_buffer);
      obj.model->draw(command_buffer);
    }
  }

 private:
  void createPipelineLayout() {
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutInfo, nullptr, &pipeline_layout_) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create pipeline layout");
    }
  }

  void createPipeline(VkRenderPass render_pass) {
    assert(pipeline_layout_ != nullptr && "Cannot create pipeline before pipeline layout");
    PipelineConfigInfo pipelineConfigInfo{};
    Pipeline::defaultPipelineConfigInfo(pipelineConfigInfo);
    pipelineConfigInfo.renderPass = render_pass;
    pipelineConfigInfo.pipelineLayout = pipeline_layout_;
    pipeline_ = std::make_unique<Pipeline>(device_, "..//..//vulkan-engine/shaders/simple_shader.vert.spv",
                                           "..//..//vulkan-engine/shaders/simple_shader.frag.spv", pipelineConfigInfo);
  }

  Device& device_;
  VkPipelineLayout pipeline_layout_;
  std::unique_ptr<Pipeline> pipeline_;
};

}  // namespace vulkan_engine::gfx
