#include "tupi/semaphore.h"

#include "tupi/logical_device.h"

namespace tupi {
Semaphore::~Semaphore() {
  vkDestroySemaphore(logical_device_->handle(), semaphore_, nullptr);
}

Semaphore::Semaphore(LogicalDevicePtr logical_device)
    : logical_device_(std::move(logical_device)) {
  VkSemaphoreCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  if (vkCreateSemaphore(logical_device_->handle(), &create_info, nullptr,
                        &semaphore_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create semaphore!");
  }
}
}  // namespace tupi
