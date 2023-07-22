#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"

namespace tupi {
class Semaphore : public internal::Creatable<Semaphore, std::shared_ptr> {
  friend class internal::Creatable<Semaphore, std::shared_ptr>;

 public:
  ~Semaphore();

  auto handle() const -> VkSemaphore;

 protected:
  Semaphore(LogicalDevicePtr logical_device);
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
