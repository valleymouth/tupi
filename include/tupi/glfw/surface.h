#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/glfw/window.h"
#include "tupi/internal/resource.h"
#include "tupi/surface.h"

namespace tupi::glfw {
class Surface : public ISurface, public internal::SharedResource<Surface> {
 public:
  Surface(Token, EnginePtr engine, WindowPtr window);
  ~Surface();

  auto handle() const -> VkSurfaceKHR override;
  auto window() const -> IWindowPtr override;

 protected:
  Surface(const Surface&) = delete;
  Surface(Surface&&) = delete;
  Surface& operator=(const Surface&) = delete;
  Surface& operator=(Surface&&) = delete;

 private:
  EnginePtr engine_{};
  WindowPtr window_{};
  VkSurfaceKHR surface_{VK_NULL_HANDLE};
};

inline auto Surface::handle() const -> VkSurfaceKHR { return surface_; }

inline auto Surface::window() const -> IWindowPtr { return window_; }
}  // namespace tupi::glfw
