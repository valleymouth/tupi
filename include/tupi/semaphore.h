#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/resource.h"

namespace tupi {
class Semaphore : public internal::SharedResource<Semaphore> {
 public:
  Semaphore(Token, LogicalDevicePtr logical_device);
  ~Semaphore();

  auto handle() const -> VkSemaphore;

  static auto handles(const SemaphorePtrVec& semaphores)
      -> std::vector<VkSemaphore>;

 protected:
  Semaphore(const Semaphore&) = delete;
  Semaphore(Semaphore&&) = delete;
  Semaphore& operator=(const Semaphore&) = delete;
  Semaphore& operator=(Semaphore&&) = delete;

 private:
  LogicalDevicePtr logical_device_{};
  VkSemaphore semaphore_{VK_NULL_HANDLE};
};

inline auto Semaphore::handle() const -> VkSemaphore { return semaphore_; }
}  // namespace tupi
