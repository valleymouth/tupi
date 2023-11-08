#include "tupi/fence.h"

namespace tupi {
Fence::Fence(Token, LogicalDevicePtr logical_device, bool create_signaled)
    : logical_device_(std::move(logical_device)) {
  VkFenceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  if (create_signaled) {
    create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  }
  if (vkCreateFence(logical_device_->handle(), &create_info, nullptr,
                    &fence_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create fence!");
  }
}

Fence::~Fence() {
  if (logical_device_) {
    vkDestroyFence(logical_device_->handle(), fence_, nullptr);
  }
}
}  // namespace tupi
