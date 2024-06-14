#pragma once

#include <memory>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class Sampler {
 public:
  explicit Sampler(LogicalDeviceSharedPtr logical_device, VkFilter mag_filter,
                   VkFilter min_filter, VkSamplerAddressMode address_mode_u,
                   VkSamplerAddressMode address_mode_t);
  ~Sampler();
  Sampler(const Sampler&) = delete;
  auto operator=(const Sampler&) -> Sampler& = delete;
  Sampler(Sampler&& other) = default;
  auto operator=(Sampler&& other) -> Sampler& = default;

  auto handle() const -> VkSampler { return sampler_; }
  auto logicalDevice() const -> LogicalDeviceSharedPtr {
    return logical_device_;
  }

 private:
  LogicalDeviceSharedPtr logical_device_{};
  Handle<VkSampler> sampler_{};
};
}  // namespace tupi