#include "tupi/glfw/window.h"

#include <iostream>

namespace tupi::glfw {
Window::Window(Token, uint32_t width, uint32_t height) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window_ = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
  glfwSetFramebufferSizeCallback(window_, resize);
}

Window::~Window() { glfwDestroyWindow(window_); }

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

auto Window::resize(GLFWwindow* window, int width, int height) -> void {
  // std::cout << "Window::resize " << width << " " << height << std::endl;
}
}  // namespace tupi::glfw
