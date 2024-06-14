#include "tupi/image.h"

#include "tupi/physical_device.h"

namespace tupi {
auto IImage::checkFormatSupport(const PhysicalDeviceSharedPtr& physical_device,
                                VkFormat format,
                                VkFormatFeatureFlags feature_flags) -> bool {
  VkFormatProperties2 format_properties{VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2};
  vkGetPhysicalDeviceFormatProperties2(physical_device->handle(), format,
                                       &format_properties);
  return (format_properties.formatProperties.optimalTilingFeatures &
          feature_flags) == feature_flags;
}
}  // namespace tupi