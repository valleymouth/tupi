#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/glfw/window.h"
#include "tupi/internal/creatable.h"
#include "tupi/surface.h"

namespace tupi::glfw {
class Surface : public ISurface,
                public internal::Creatable<Surface, std::shared_ptr> {
  friend class Creatable<Surface, std::shared_ptr>;

 public:
  ~Surface();

  auto handle() const -> VkSurfaceKHR override;
  auto window() const -> IWindowPtr override;

 protected:
  Surface(EnginePtr engine, WindowPtr window);
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
