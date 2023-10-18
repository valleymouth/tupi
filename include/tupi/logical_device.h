#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "tupi/extension_set.h"
#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"
#include "tupi/queue_family.h"

namespace tupi {
class LogicalDevice
    : public internal::Creatable<LogicalDevice, std::shared_ptr> {
  friend class internal::Creatable<LogicalDevice, std::shared_ptr>;

 public:
  struct QueueCreateInfo {
    QueueFamily queue_family{};
    std::vector<float> priorities{};
  };
  using QueueCreateInfoList = std::vector<QueueCreateInfo>;

  ~LogicalDevice();

  auto physicalDevice() const -> PhysicalDevicePtr;
  auto handle() const -> VkDevice;
  auto waitIdle() const -> void;
  auto findMemoryType(const VkMemoryRequirements& requirements,
                      VkMemoryPropertyFlags property_flags) const -> uint32_t;

 protected:
  LogicalDevice(PhysicalDevicePtr physical_device,
                const QueueCreateInfoList& queue_create_infos,
                const ExtensionSet& extensions = {});
  LogicalDevice(const LogicalDevice&) = delete;
  LogicalDevice(LogicalDevice&&) = delete;
  LogicalDevice& operator=(const LogicalDevice&) = delete;
  LogicalDevice& operator=(LogicalDevice&&) = delete;

 private:
  PhysicalDevicePtr physical_device_{};
  VkDevice device_{VK_NULL_HANDLE};
};

inline auto LogicalDevice::physicalDevice() const -> PhysicalDevicePtr {
  return physical_device_;
}

inline auto LogicalDevice::handle() const -> VkDevice { return device_; }

inline auto LogicalDevice::waitIdle() const -> void {
  vkDeviceWaitIdle(device_);
}
}  // namespace tupi
