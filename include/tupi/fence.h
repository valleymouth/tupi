#pragma once

#include <memory>

#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/logical_device.h"

namespace tupi {
class Fence {
 public:
  Fence(LogicalDeviceSharedPtr logical_device, bool create_signaled = false);
  ~Fence();
  Fence(const Fence&) = delete;
  Fence& operator=(const Fence&) = delete;
  Fence(Fence&& other) = default;
  Fence& operator=(Fence&& other) = default;

  auto handle() const -> VkFence { return fence_; }
  auto wait() const -> void {
    vkWaitForFences(logical_device_->handle(), 1, &fence_, VK_TRUE, UINT64_MAX);
  }
  auto reset() const -> void {
    vkResetFences(logical_device_->handle(), 1, &fence_);
  }

 private:
  LogicalDeviceSharedPtr logical_device_{};
  Handle<VkFence> fence_{};
};
}  // namespace tupi
