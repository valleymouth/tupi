#include "tupi/glfw/surface.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "tupi/engine.h"

namespace tupi::glfw {
Surface::~Surface() {
  vkDestroySurfaceKHR(engine_->handle(), surface_, nullptr);
}

Surface::Surface(EnginePtr engine, WindowPtr window)
    : engine_(std::move(engine)), window_(std::move(window)) {
  auto glfw_window = std::dynamic_pointer_cast<Window>(window_);
  if (!glfw_window) {
    throw std::runtime_error(
        "tupi::glfw::Surface must be called with an instance of "
        "tupi::glfw::Window!");
  }
  if (glfwCreateWindowSurface(engine_->handle(), glfw_window->handle(), nullptr,
                              &surface_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface!");
  }
}
}  // namespace tupi::glfw
