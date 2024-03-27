//
// Created by marvi on 16.03.2024.
//

#pragma once

#include <cassert>
#include "device.h"
#include "swap_chain.h"
#include "window.h"

namespace vulkan_engine::gfx {
class Renderer {
 public:
  Renderer(Window& window, Device& device);
  ~Renderer();
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  bool isFrameInProgress() const { return is_frame_started_; }
  VkRenderPass getSwapChainRenderPass() const {
    return swap_chain_->getRenderPass();
  }
  float getAspectRatio() const { return swap_chain_->extentAspectRatio(); }
  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(is_frame_started_ &&
           "Cannot get command buffer when frame not started.");
    return command_buffers_[current_frame_index_];
  }

  uint32_t getFrameIndex() const {
    assert(is_frame_started_ &&
           "Cannot get frame index when frame not started.");
    return current_frame_index_;
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer command_buffer) const;
  void endSwapChainRenderPass(VkCommandBuffer command_buffer) const;

 private:
  Window& window_;
  Device& device_;
  std::unique_ptr<SwapChain> swap_chain_;
  std::vector<VkCommandBuffer> command_buffers_;
  uint32_t current_image_index_{0};
  uint32_t current_frame_index_{0};
  bool is_frame_started_{false};

  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();
};
}  // namespace vulkan_engine::gfx
