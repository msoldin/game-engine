//
// Created by marvi on 16.03.2024.
//

#include "model.h"

#include <cstring>

namespace vulkan_engine::gfx {
Model::Model(Device &device, const std::vector<Vertex> &vertices)
    : m_device(device) {
  createVertexBuffer(vertices);
}

Model::~Model() {
  vkDestroyBuffer(m_device.device(), m_vertexBuffer, nullptr);
  vkFreeMemory(m_device.device(), m_vertexBufferMemory, nullptr);
}

void Model::createVertexBuffer(const std::vector<Vertex> &vertices) {
  m_vertexCount = static_cast<uint32_t>(vertices.size());
  assert(m_vertexCount >= 3 && "Vertex count must be at least 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * m_vertexCount;

  m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        m_vertexBuffer, m_vertexBufferMemory);

  void *data;
  vkMapMemory(m_device.device(), m_vertexBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(m_device.device(), m_vertexBufferMemory);
}

void Model::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {m_vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void Model::draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
}

std::vector<VkVertexInputBindingDescription>
Model::Vertex::getBindingDescriptions() {
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof(Vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return {bindingDescription};
}

std::vector<VkVertexInputAttributeDescription>
Model::Vertex::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
  // Position attribute
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, m_position);
  ;

  // Color attribute
  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, m_color);
  return attributeDescriptions;
}
} // namespace vulkan_engine::gfx
