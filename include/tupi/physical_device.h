#pragma once

#include <memory>
#include <ranges>
#include <vulkan/vulkan.hpp>

#include "tupi/extension_set.h"
#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"

namespace tupi {
class PhysicalDevice
    : public internal::Creatable<PhysicalDevice, std::shared_ptr> {
  friend class internal::Creatable<PhysicalDevice, std::shared_ptr>;

 public:
  enum class Feature { SamplerAnisotropy };

  auto handle() const -> VkPhysicalDevice;
  auto deviceType() const -> VkPhysicalDeviceType;
  auto hasExtension(const std::string& name) const -> bool;
  auto hasFeature(Feature feature) const -> bool;
  auto memoryProperties() const -> VkPhysicalDeviceMemoryProperties;
  auto maxSamplerAnisotropy() const -> uint32_t;
  auto findSupportedFormat(const std::vector<VkFormat>& candidates,
                           VkImageTiling tiling, VkFormatFeatureFlags features)
      -> VkFormat;
  auto findDepthFormat() -> VkFormat;

  static auto enumerate(const EnginePtr& engine) -> PhysicalDevicePtrVec;
  static auto hasStencilComponent(VkFormat format) -> bool;

 protected:
  PhysicalDevice() = default;

 private:
  EnginePtr engine_{};
  VkPhysicalDevice physical_device_{VK_NULL_HANDLE};
  VkPhysicalDeviceProperties properties_{};
  VkPhysicalDeviceFeatures features_{};
  ExtensionSet extensions_{};
};

inline auto PhysicalDevice::handle() const -> VkPhysicalDevice {
  return physical_device_;
}

inline auto PhysicalDevice::deviceType() const -> VkPhysicalDeviceType {
  return properties_.deviceType;
}

inline auto PhysicalDevice::hasExtension(const std::string& name) const
    -> bool {
  return extensions_.hasExtension(name);
}

inline auto PhysicalDevice::memoryProperties() const
    -> VkPhysicalDeviceMemoryProperties {
  VkPhysicalDeviceMemoryProperties properties;
  vkGetPhysicalDeviceMemoryProperties(physical_device_, &properties);
  return properties;
}

inline auto PhysicalDevice::maxSamplerAnisotropy() const -> uint32_t {
  return properties_.limits.maxSamplerAnisotropy;
}

inline auto hasExtension(const std::string& name) {
  return std::views::filter(
      [name](const auto& x) { return x->hasExtension(name); });
}

inline auto hasFeature(PhysicalDevice::Feature feature) {
  return std::views::filter(
      [=](const auto& x) { return x->hasFeature(feature); });
}
}  // namespace tupi
