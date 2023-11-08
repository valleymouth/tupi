#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/resource.h"

namespace tupi {
class CommandPool : public internal::SharedResource<CommandPool> {
 public:
  CommandPool(Token, LogicalDevicePtr logical_device,
              const QueueFamily& queue_family);
  ~CommandPool();

  auto handle() const -> VkCommandPool;
  auto logicalDevice() const -> LogicalDevicePtr;

 protected:
  CommandPool(const CommandPool&) = delete;
  CommandPool(CommandPool&&) = delete;
  CommandPool& operator=(const CommandPool&) = delete;
  CommandPool& operator=(CommandPool&&) = delete;

 private:
  LogicalDevicePtr logical_device_{};
  VkCommandPool command_pool_{VK_NULL_HANDLE};
};

inline auto CommandPool::handle() const -> VkCommandPool {
  return command_pool_;
}

inline auto CommandPool::logicalDevice() const -> LogicalDevicePtr {
  return logical_device_;
}
}  // namespace tupi
