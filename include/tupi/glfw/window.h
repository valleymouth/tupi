#pragma once

#include <memory>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "tupi/internal/resource.h"
#include "tupi/window.h"

namespace tupi::glfw {
class Window : public IWindow, public internal::SharedResource<Window> {
 public:
  Window(Token, uint32_t width, uint32_t height);
  ~Window();

  auto handle() const -> GLFWwindow*;
  auto extent() const -> VkExtent2D override;

 protected:
  Window(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(const Window&) = delete;
  Window& operator=(Window&&) = delete;

 private:
  static auto resize(GLFWwindow* window, int width, int height) -> void;

  GLFWwindow* window_;
};

inline auto Window::handle() const -> GLFWwindow* { return window_; }
}  // namespace tupi::glfw
