#include "tupi/glfw/window.h"

#include <iostream>

#include "tupi/glfw/input.h"

namespace tupi::glfw {
Window::Window(uint32_t width, uint32_t height) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window_ = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
  glfwSetFramebufferSizeCallback(window_, resize);
  glfwSetWindowUserPointer(window_, this);

  glfwSetKeyCallback(window_, keyCallback);
  addKeyboardObserver(std::make_shared<KeyboardObserver>());

  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (glfwRawMouseMotionSupported()) {
    glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }

  glfwSetCursorPosCallback(window_, mouseMoveCallback);
  glfwSetMouseButtonCallback(window_, mouseButtonCallback);
  addMouseObserver(std::make_shared<MouseObserver>());
}

Window::~Window() { glfwDestroyWindow(window_); }

Window::Window(Window&& other) { *this = std::move(other); }

Window& Window::operator=(Window&& other) {
  window_ = other.window_;
  other.window_ = nullptr;
  return *this;
}

auto Window::extent() const -> VkExtent2D {
  int width = 0;
  int height = 0;
  glfwGetFramebufferSize(window_, &width, &height);
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(window_, &width, &height);
    glfwWaitEvents();
  }
  return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

auto Window::addKeyboardObserver(IKeyboardObserverSharedPtr keyboard_observer)
    -> void {
  keyboard_observers_.push_back(std::move(keyboard_observer));
}

auto Window::addMouseObserver(IMouseObserverSharedPtr mouse_observer) -> void {
  mouse_observers_.push_back(std::move(mouse_observer));
}

auto Window::keyCallback(GLFWwindow* glfw_window, int key, int scancode,
                         int action, int modifiers) -> void {
  auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
  for (const auto& keyboard_observer : window->keyboard_observers_) {
    switch (action) {
      case GLFW_PRESS:
        keyboard_observer->onKeyPressed(glfw_window, key, modifiers);
        break;
      case GLFW_RELEASE:
        keyboard_observer->onKeyReleased(glfw_window, key, modifiers);
        break;
      case GLFW_REPEAT:
        keyboard_observer->onKeyRepeated(glfw_window, key, modifiers);
        break;
    }
  }
}

auto Window::mouseMoveCallback(GLFWwindow* glfw_window, double x, double y)
    -> void {
  auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
  for (const auto& mouse_observer : window->mouse_observers_) {
    mouse_observer->onMouseMove(glfw_window, x, y);
  }
}

auto Window::mouseButtonCallback(GLFWwindow* glfw_window, int button,
                                 int action, int modifiers) -> void {
  auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
  for (const auto& mouse_observer : window->mouse_observers_) {
    switch (action) {
      case GLFW_PRESS:
        mouse_observer->onMousePress(glfw_window, button, modifiers);
        break;
      case GLFW_RELEASE:
        mouse_observer->onMouseRelease(glfw_window, button, modifiers);
        break;
    }
  }
}

auto Window::resize(GLFWwindow* window, int width, int height) -> void {
  // std::cout << "Window::resize " << width << " " << height << std::endl;
}
}  // namespace tupi::glfw
