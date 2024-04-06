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
};
}  // namespace tupi
