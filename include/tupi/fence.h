#pragma once

#include <memory>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"
#include "tupi/logical_device.h"

namespace tupi {
class Fence : public internal::Creatable<Fence, std::shared_ptr> {
  friend class internal::Creatable<Fence, std::shared_ptr>;

 public:
  ~Fence();

  auto handle() const -> VkFence;
  auto wait() const -> void;
  auto reset() const -> void;

 protected:
  Fence(LogicalDevicePtr logical_device, bool create_signaled = false);
  Fence(const Fence&) = delete;
  Fence(Fence&&) = delete;
  Fence& operator=(const Fence&) = delete;
  Fence& operator=(Fence&&) = delete;

 private:
  LogicalDevicePtr logical_device_{};
  VkFence fence_{VK_NULL_HANDLE};
};

inline auto Fence::handle() const -> VkFence { return fence_; }

inline auto Fence::wait() const -> void {
  vkWaitForFences(logical_device_->handle(), 1, &fence_, VK_TRUE, UINT64_MAX);
}

inline auto Fence::reset() const -> void {
  vkResetFences(logical_device_->handle(), 1, &fence_);
}
}  // namespace tupi
