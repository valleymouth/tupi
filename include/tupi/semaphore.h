#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class Semaphore {
 public:
  explicit Semaphore(LogicalDeviceSharedPtr logical_device);
  ~Semaphore();
  Semaphore(const Semaphore&) = delete;
  Semaphore& operator=(const Semaphore&) = delete;
  Semaphore(Semaphore&&) = default;
  Semaphore& operator=(Semaphore&&) = default;

  auto handle() const -> VkSemaphore { return semaphore_; }
  static auto handles(const SemaphorePtrVec& semaphores)
      -> std::vector<VkSemaphore>;

 private:
  LogicalDeviceSharedPtr logical_device_{};
  Handle<VkSemaphore> semaphore_{};
};
}  // namespace tupi
