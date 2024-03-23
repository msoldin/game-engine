//
// Created by marvi on 16.03.2024.
//

#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "device.h"

namespace vulkan_engine::gfx {
class Model {
public:
  struct Vertex {
    glm::vec2 m_position;
    glm::vec3 m_color;

    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();

    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions();
  };

  Model(Device &device, const std::vector<Vertex> &vertices);

  ~Model();

  Model(const Model &) = delete;

  Model &operator=(const Model &) = delete;

  void bind(VkCommandBuffer commandBuffer);

  void draw(VkCommandBuffer commandBuffer);

private:
  Device &m_device;
  VkBuffer m_vertexBuffer;
  VkDeviceMemory m_vertexBufferMemory;
  uint32_t m_vertexCount;

  void createVertexBuffer(const std::vector<Vertex> &vertices);
};
} // namespace vulkan_engine::gfx
