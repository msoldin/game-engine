//
// Created by marvi on 16.03.2024.
//

#include "renderer.h"
#include <glog/logging.h>

namespace vulkan_engine::gfx {

Renderer::Renderer(Window& window, Device& device)
    : window_(window), device_(device) {
  recreateSwapChain();
  createCommandBuffers();
}

Renderer::~Renderer() {
  freeCommandBuffers();
}

void Renderer::recreateSwapChain() {
  auto extent = window_.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = window_.getExtent();
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(device_.device());
  if (swap_chain_ == nullptr) {
    swap_chain_ = std::make_unique<SwapChain>(device_, extent);
  } else {
    std::shared_ptr<SwapChain> old_swap_chain = std::move(swap_chain_);
    swap_chain_ = std::make_unique<SwapChain>(device_, extent, old_swap_chain);

    if (!old_swap_chain->compareSwapFormats(*swap_chain_)) {
      throw std::runtime_error("Swap chain image or depth format has changed");
    }
  }
}

void Renderer::createCommandBuffers() {
  command_buffers_.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = device_.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(command_buffers_.size());
  if (vkAllocateCommandBuffers(device_.device(), &allocInfo,
                               command_buffers_.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate command buffers");
  }
}

void Renderer::freeCommandBuffers() {
  vkFreeCommandBuffers(device_.device(), device_.getCommandPool(),
                       static_cast<uint32_t>(command_buffers_.size()),
                       command_buffers_.data());
  command_buffers_.clear();
}

VkCommandBuffer Renderer::beginFrame() {
  assert(!is_frame_started_ &&
         "Can't start new frame while already in progress");
  const auto result = swap_chain_->acquireNextImage(&current_image_index_);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return nullptr;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("Failed to acquire next image");
  }

  is_frame_started_ = true;

  const auto command_buffer = getCurrentCommandBuffer();
  VkCommandBufferBeginInfo begin_info{};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin recording command buffer");
  }
  return command_buffer;
}

void Renderer::endFrame() {
  assert(is_frame_started_ && "Can't end frame while frame is not in progress");

  auto command_buffer = getCurrentCommandBuffer();
  if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
    throw std::runtime_error("Failed to record command buffer");
  }

  const auto result =
      swap_chain_->submitCommandBuffers(&command_buffer, &current_image_index_);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      window_.wasWindowResized()) {
    window_.resetWindowResizedFlag();
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to present swap chain image!");
  }

  is_frame_started_ = false;
  current_frame_index_ =
      (current_frame_index_ + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::beginSwapChainRenderPass(VkCommandBuffer command_buffer) const {
  assert(is_frame_started_ &&
         "Can't begin render pass when frame not started.");
  assert(
      command_buffer == getCurrentCommandBuffer() &&
      "Can't begin render pass on command buffer from outside current frame.");

  VkRenderPassBeginInfo render_pass_begin_info{};
  render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_begin_info.renderPass = swap_chain_->getRenderPass();
  render_pass_begin_info.framebuffer =
      swap_chain_->getFrameBuffer(current_image_index_);
  render_pass_begin_info.renderArea.offset = {0, 0};
  render_pass_begin_info.renderArea.extent = swap_chain_->getSwapChainExtent();

  std::array<VkClearValue, 2> clear_values{};
  clear_values[0].color = {0.0f, 0.0f, 0.00f, 1.0f};
  clear_values[1].depthStencil = {1.0f, 0};
  render_pass_begin_info.clearValueCount =
      static_cast<uint32_t>(clear_values.size());
  render_pass_begin_info.pClearValues = clear_values.data();

  vkCmdBeginRenderPass(command_buffer,
                       &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swap_chain_->width());
  viewport.height = static_cast<float>(swap_chain_->height());
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, swap_chain_->getSwapChainExtent()};
  vkCmdSetViewport(command_buffer, 0, 1, &viewport);
  vkCmdSetScissor(command_buffer, 0, 1, &scissor);
}

void Renderer::endSwapChainRenderPass(VkCommandBuffer command_buffer) const {
  assert(is_frame_started_ && "Can't end render pass when frame not started.");
  assert(command_buffer == getCurrentCommandBuffer() &&
         "Can't end render pass on command buffer from outside current frame.");
  vkCmdEndRenderPass(command_buffer);
}

}  // namespace vulkan_engine::gfx