//
// Created by marvi on 13.03.2024.
//

#include "Window.h"

#include <stdexcept>

namespace VulkanEngine {
    Window::Window(int width, int heigh, std::string title) : m_width(width),
                                                              m_height(heigh),
                                                              m_windowName(title) {
        initWindow();
    }

    Window::~Window() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        m_window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), nullptr, nullptr);
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(m_window);
    }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface");
        }
    }
}
