//
// Created by marvi on 13.03.2024.
//

module;
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>

export module vulkan_engine.gfx:window;

namespace vulkan_engine::gfx {
export class Window {
 public:
  Window(int width, int heigh, std::string title) : m_width(width), m_height(heigh), m_windowName(title) { initWindow(); }

  ~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }

  Window(const Window &) = delete;

  Window &operator=(const Window &) = delete;

  bool shouldClose() const { return glfwWindowShouldClose(m_window); }

  bool wasWindowResized() const { return m_framebufferResized; };
  void resetWindowResizedFlag() { m_framebufferResized = false; }

  VkExtent2D getExtent() const { return {static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)}; }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) const {
    if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create window surface");
    }
  }

 private:
  static void framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto app = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    app->m_framebufferResized = true;
    app->m_width = width;
    app->m_height = height;
  }

  int m_width;
  int m_height;
  bool m_framebufferResized = false;

  GLFWwindow *m_window;

  std::string m_windowName;

  void initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    m_window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
  }
};
}  // namespace vulkan_engine::gfx
