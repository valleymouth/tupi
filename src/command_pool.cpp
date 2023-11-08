#include "tupi/command_pool.h"

#include "tupi/logical_device.h"
#include "tupi/queue_family.h"

namespace tupi {
CommandPool::CommandPool(Token, LogicalDevicePtr logical_device,
                         const QueueFamily& queue_family)
    : logical_device_(std::move(logical_device)) {
  VkCommandPoolCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  create_info.queueFamilyIndex = queue_family.index();

  if (vkCreateCommandPool(logical_device_->handle(), &create_info, nullptr,
                          &command_pool_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
  }
}

CommandPool::~CommandPool() {
  vkDestroyCommandPool(logical_device_->handle(), command_pool_, nullptr);
}
}  // namespace tupi
