//
// Created by marvi on 13.03.2024.
//

#include "window.h"

#include <stdexcept>

namespace vulkan_engine::gfx {
Window::Window(int width, int heigh, std::string title)
  : m_width(width), m_height(heigh), m_windowName(title) {
  initWindow();
}

Window::~Window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Window::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  m_window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), nullptr,
                              nullptr);
  glfwSetWindowUserPointer(m_window, this);
  glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

bool Window::shouldClose() const { return glfwWindowShouldClose(m_window); }

void Window::createWindowSurface(VkInstance instance,
                                 VkSurfaceKHR *surface) const {
  if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface");
  }
}

void Window::framebufferResizeCallback(GLFWwindow *window, int width,
                                       int height) {
  auto app = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
  app->m_framebufferResized = true;
  app->m_width = width;
  app->m_height = height;
}
} // namespace vulkan_engine::gfx
