//
// Created by marvi on 13.03.2024.
//

#pragma once
#define GLFW_INCLUDE_VULKAN
#include <string>
#include <GLFW/glfw3.h>

namespace VulkanEngine {
    class Window {
    public:
        Window(int width, int heigh, std::string title);

        ~Window();

        Window(const Window &) = delete;

        Window &operator =(const Window &) = delete;

        bool shouldClose() const;

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        const int m_width;

        const int m_height;

        GLFWwindow *m_window;

        std::string m_windowName;

        void initWindow();
    };
}
