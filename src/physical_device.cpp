#include "tupi/physical_device.h"

#include "tupi/engine.h"

namespace tupi {
auto PhysicalDevice::hasFeature(Feature feature) const -> bool {
  switch (feature) {
    case Feature::SamplerAnisotropy:
      return features_.samplerAnisotropy == VK_TRUE;
    default:
      return false;
  }
}

VkFormat PhysicalDevice::findSupportedFormat(
    const std::vector<VkFormat>& candidates, VkImageTiling tiling,
    VkFormatFeatureFlags features) {
  for (const auto& format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physical_device_, format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
               (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  throw std::runtime_error("Failed to find supported format!");
}

VkFormat PhysicalDevice::findDepthFormat() {
  return findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

auto PhysicalDevice::enumerate(EngineSharedPtr engine)
    -> PhysicalDeviceSharedPtrVec {
  uint32_t count = 0;
  vkEnumeratePhysicalDevices(engine->handle(), &count, nullptr);
  std::vector<VkPhysicalDevice> devices(count);
  vkEnumeratePhysicalDevices(engine->handle(), &count, devices.data());
  std::vector<PhysicalDeviceSharedPtr> result;
  result.reserve(count);
  for (const auto& vk_device : devices) {
    auto& device = result.emplace_back(std::make_shared<PhysicalDevice>());
    device->engine_ = engine;
    device->physical_device_ = vk_device;
    vkGetPhysicalDeviceProperties(vk_device, &device->properties_);
    vkGetPhysicalDeviceFeatures(vk_device, &device->features_);
    device->extensions_ = ExtensionSet::enumerate(device);
  }
  return result;
}

auto PhysicalDevice::hasStencilComponent(VkFormat format) -> bool {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
         format == VK_FORMAT_D24_UNORM_S8_UINT;
}
}  // namespace tupi
