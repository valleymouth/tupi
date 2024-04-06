#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "tupi/window.h"

namespace tupi::glfw {
class Window : public IWindow {
 public:
  Window(uint32_t width, uint32_t height);
  ~Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  Window(Window&& other);
  Window& operator=(Window&& other);

  auto handle() const -> GLFWwindow* { return window_; }
  auto extent() const -> VkExtent2D override;

 private:
  static auto resize(GLFWwindow* window, int width, int height) -> void;

  GLFWwindow* window_;
};
}  // namespace tupi::glfw
