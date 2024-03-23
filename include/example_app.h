//
// Created by marvi on 14.03.2024.
//

#pragma once

#include "device.h"
#include "game_object.h"
#include "renderer.h"
#include "window.h"

namespace vulkan_engine::gfx {
class ExampleApp {
 public:
  static constexpr int WIDHT = 800;
  static constexpr int HEIGHT = 600;

  ExampleApp();
  ~ExampleApp();
  ExampleApp(const ExampleApp&) = delete;
  ExampleApp& operator=(const ExampleApp&) = delete;

  void run();

 private:
  Window window_{WIDHT, HEIGHT, "Vulkan"};
  Device device_{window_};
  Renderer renderer_{window_, device_};
  std::vector<GameObject> game_objects_;

  void loadGameObjects();
};
}  // namespace vulkan_engine::gfx
