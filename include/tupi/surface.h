#pragma once

#include <vulkan/vulkan.hpp>

namespace tupi {
class ISurface {
 public:
  virtual ~ISurface() = default;

  virtual auto handle() const -> VkSurfaceKHR = 0;
  virtual auto window() const -> IWindowPtr = 0;
};
}  // namespace tupi
