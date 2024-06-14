#pragma once

#include <ranges>
#include <vulkan/vulkan.hpp>

#include "tupi/extension_set.h"
#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class PhysicalDevice {
 public:
  enum class Feature { SamplerAnisotropy };

  auto handle() const -> VkPhysicalDevice { return physical_device_; }

  auto deviceType() const -> VkPhysicalDeviceType {
    return properties_.deviceType;
  }

  auto hasExtension(const std::string& name) const -> bool {
    return extensions_.hasExtension(name);
  }

  auto hasBindlessSupport() const -> bool {
    return indexing_features_.descriptorBindingPartiallyBound &&
           indexing_features_.runtimeDescriptorArray;
  }

  auto indexingFeatures() const -> VkPhysicalDeviceDescriptorIndexingFeatures {
    return indexing_features_;
  }

  auto memoryProperties() const -> VkPhysicalDeviceMemoryProperties {
    VkPhysicalDeviceMemoryProperties properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device_, &properties);
    return properties;
  }

  auto maxSamplerAnisotropy() const -> uint32_t {
    return properties_.limits.maxSamplerAnisotropy;
  }

  auto hasFeature(Feature feature) const -> bool;
  auto findSupportedFormat(const std::vector<VkFormat>& candidates,
                           VkImageTiling tiling, VkFormatFeatureFlags features)
      -> VkFormat;
  auto findDepthFormat() -> VkFormat;

  static auto enumerate(EngineSharedPtr engine) -> PhysicalDeviceSharedPtrVec;
  static auto hasStencilComponent(VkFormat format) -> bool;

 private:
  EngineSharedPtr engine_{};
  Handle<VkPhysicalDevice> physical_device_{};
  VkPhysicalDeviceProperties properties_{};
  VkPhysicalDeviceFeatures features_{};
  ExtensionSet extensions_{};
  VkPhysicalDeviceDescriptorIndexingFeatures indexing_features_{
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES, nullptr};
  bool bindless_supported_{false};
};

inline auto hasExtension(const std::string& name) {
  return std::views::filter(
      [name](const auto& x) { return x->hasExtension(name); });
}

inline auto hasFeature(PhysicalDevice::Feature feature) {
  return std::views::filter(
      [=](const auto& x) { return x->hasFeature(feature); });
}
}  // namespace tupi
