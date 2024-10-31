#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>

#include "tupi/fwd.h"
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

  auto addKeyboardObserver(IKeyboardObserverSharedPtr keyboard_observer)
      -> void override;
  auto addMouseObserver(IMouseObserverSharedPtr mouse_observer)
      -> void override;

 private:
  static auto keyCallback(GLFWwindow* glfw_window, int key, int scancode,
                          int action, int modifiers) -> void;
  static auto mouseMoveCallback(GLFWwindow* glfw_window, double x, double y)
      -> void;
  static auto mouseButtonCallback(GLFWwindow* glfw_window, int button,
                                  int action, int modifiers) -> void;
  static auto resize(GLFWwindow* window, int width, int height) -> void;

  GLFWwindow* window_{};
  IKeyboardObserverSharedPtrVec keyboard_observers_{};
  IMouseObserverSharedPtrVec mouse_observers_{};
};
}  // namespace tupi::glfw
