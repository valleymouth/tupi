#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "tupi/extension_set.h"
#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/queue_family.h"

namespace tupi {
struct QueueCreateInfo {
  QueueFamily queue_family{};
  std::vector<float> priorities{};
};

class LogicalDevice {
 public:
  LogicalDevice(PhysicalDeviceSharedPtr physical_device,
                const QueueCreateInfoVec& queue_create_infos,
                const ExtensionSet& extensions = {});
  ~LogicalDevice();
  LogicalDevice(const LogicalDevice&) = delete;
  LogicalDevice& operator=(const LogicalDevice&) = delete;
  LogicalDevice(LogicalDevice&& other) = default;
  LogicalDevice& operator=(LogicalDevice&& other) = default;

  auto physicalDevice() const -> PhysicalDeviceSharedPtr {
    return physical_device_;
  }

  auto handle() const -> VkDevice { return device_; }

  auto waitIdle() const -> void { vkDeviceWaitIdle(device_); }

  auto findMemoryType(const VkMemoryRequirements& requirements,
                      VkMemoryPropertyFlags property_flags) const -> uint32_t;

 private:
  PhysicalDeviceSharedPtr physical_device_{};
  Handle<VkDevice> device_{};
};
}  // namespace tupi
