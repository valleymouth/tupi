#pragma once

#include <memory>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "tupi/internal/creatable.h"
#include "tupi/window.h"

namespace tupi::glfw {
class Window : public IWindow,
               public internal::Creatable<Window, std::shared_ptr> {
  friend class internal::Creatable<Window, std::shared_ptr>;

 public:
  ~Window();

  auto handle() const -> GLFWwindow*;
  auto extent() const -> VkExtent2D override;

 protected:
  Window(uint32_t width, uint32_t height);
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
