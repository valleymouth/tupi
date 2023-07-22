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
  auto handle() const -> VkPhysicalDevice;
  auto deviceType() const -> VkPhysicalDeviceType;
  auto hasExtension(const std::string& name) const -> bool;

  static auto enumerate(const EnginePtr& engine) -> PhysicalDevicePtrVec;

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

inline auto hasExtension(const std::string& name) {
  return std::views::filter(
      [name](const auto& x) { return x->hasExtension(name); });
}
}  // namespace tupi
