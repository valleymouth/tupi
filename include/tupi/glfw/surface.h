#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/glfw/window.h"
#include "tupi/handle.h"
#include "tupi/surface.h"

namespace tupi::glfw {
class Surface : public ISurface {
 public:
  Surface(EngineSharedPtr engine, WindowSharedPtr window);
  ~Surface();
  Surface(const Surface&) = delete;
  Surface& operator=(const Surface&) = delete;
  Surface(Surface&& other) = default;
  Surface& operator=(Surface&& other) = default;

  auto handle() const -> VkSurfaceKHR override { return surface_; }
  auto window() const -> IWindowPtr override { return window_; }

 private:
  EngineSharedPtr engine_{};
  WindowSharedPtr window_{};
  Handle<VkSurfaceKHR> surface_{};
};
}  // namespace tupi::glfw
