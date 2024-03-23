//
// Created by marvi on 23.03.2024.
//

#pragma once
#include "device.h"
#include "game_object.h"
#include "pipeline.h"

namespace vulkan_engine::gfx {
class RenderSystem {
public:
  RenderSystem(Device &device, VkRenderPass render_pass);
  ~RenderSystem();
  RenderSystem(const RenderSystem &) = delete;
  RenderSystem &operator=(const RenderSystem &) = delete;

  void renderGameObjects(VkCommandBuffer command_buffer,
                         std::vector<GameObject> &game_objects);

private:
  void createPipelineLayout();

  void createPipeline(VkRenderPass render_pass);

  Device &device_;
  VkPipelineLayout pipeline_layout_;
  std::unique_ptr<Pipeline> pipeline_;
};
} // namespace vulkan_engine::gfx