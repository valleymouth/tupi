#ifndef TUPI_SURFACE_H
#define TUPI_SURFACE_H

#include <vulkan/vulkan.hpp>

namespace tupi {
class ISurface {
 public:
  virtual ~ISurface() = default;

  virtual auto handle() const -> VkSurfaceKHR = 0;
  virtual auto window() const -> IWindowSharedPtr = 0;
};
}  // namespace tupi
#endif  // TUPI_SURFACE_H