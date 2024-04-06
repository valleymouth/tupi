#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class CommandPool {
 public:
  CommandPool(LogicalDeviceSharedPtr logical_device,
              const QueueFamily& queue_family);
  ~CommandPool();
  CommandPool(const CommandPool&) = delete;
  CommandPool& operator=(const CommandPool&) = delete;
  CommandPool(CommandPool&&) = default;
  CommandPool& operator=(CommandPool&&) = default;

  auto handle() const -> VkCommandPool { return command_pool_; }
  auto logicalDevice() const -> LogicalDeviceSharedPtr {
    return logical_device_;
  }

 private:
  LogicalDeviceSharedPtr logical_device_{};
  Handle<VkCommandPool> command_pool_{};
};
}  // namespace tupi
