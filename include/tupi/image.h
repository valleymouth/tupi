#pragma once

#include <filesystem>
#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class IImage {
 public:
  virtual auto handle() const -> VkImage = 0;
  virtual auto logicalDevice() const -> LogicalDeviceSharedPtr = 0;
  virtual auto is2D() const -> bool = 0;
  virtual auto format() const -> VkFormat = 0;
  static auto checkFormatSupport(const PhysicalDeviceSharedPtr& physical_device,
                                 VkFormat format,
                                 VkFormatFeatureFlags feature_flags) -> bool;
};
}  // namespace tupi
