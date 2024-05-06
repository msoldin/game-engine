//
// Created by marvi on 13.03.2024.
//

#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace vulkan_engine::gfx {
class Window {
public:
  Window(int width, int heigh, std::string title);

  ~Window();

  Window(const Window &) = delete;

  Window &operator=(const Window &) = delete;

  bool shouldClose() const;

  bool wasWindowResized() const { return m_framebufferResized; };
  void resetWindowResizedFlag() { m_framebufferResized = false; }

  VkExtent2D getExtent() const {
    return {static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)};
  }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) const;

private:
  static void framebufferResizeCallback(GLFWwindow *window, int width,
                                        int height);

  int m_width;
  int m_height;
  bool m_framebufferResized = false;

  GLFWwindow *m_window;

  std::string m_windowName;

  void initWindow();
};
} // namespace vulkan_engine::gfx
