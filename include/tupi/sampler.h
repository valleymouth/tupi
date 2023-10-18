#pragma once

#include <memory>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"

namespace tupi {
class Sampler : public internal::Creatable<Sampler, std::shared_ptr> {
  friend class internal::Creatable<Sampler, std::shared_ptr>;

 public:
  ~Sampler();

  auto handle() const -> VkSampler;
  auto logicalDevice() const -> LogicalDevicePtr;

 protected:
  Sampler(LogicalDevicePtr logical_device);
  Sampler(const Sampler&) = delete;
  Sampler(Sampler&& other);
  auto operator=(const Sampler&) -> Sampler& = delete;
  auto operator=(Sampler&& other) -> Sampler&;

 private:
  LogicalDevicePtr logical_device_{};
  VkSampler sampler_{VK_NULL_HANDLE};
};

inline auto Sampler::handle() const -> VkSampler { return sampler_; }

inline auto Sampler::logicalDevice() const -> LogicalDevicePtr {
  return logical_device_;
}
}  // namespace tupi