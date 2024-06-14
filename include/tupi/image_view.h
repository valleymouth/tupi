#ifndef TUPI_IMAGE_VIEW_H
#define TUPI_IMAGE_VIEW_H

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class IImageView {
 public:
  virtual auto handle() const -> VkImageView = 0;
  virtual auto logicalDevice() const -> LogicalDeviceSharedPtr = 0;
  virtual auto is2D() const -> bool = 0;
};
}  // namespace tupi
#endif  // TUPI_IMAGE_VIEW_H