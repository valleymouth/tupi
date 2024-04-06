#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class DescriptorPool {
 public:
  DescriptorPool(LogicalDeviceSharedPtr logical_device,
                 DescriptorPoolSizeVec pool_sizes, uint32_t max_sets);
  ~DescriptorPool();
  DescriptorPool(const DescriptorPool&) = delete;
  DescriptorPool& operator=(const DescriptorPool&) = delete;
  DescriptorPool(DescriptorPool&&) = default;
  DescriptorPool& operator=(DescriptorPool&&) = default;

  auto handle() const -> VkDescriptorPool { return descriptor_pool_; }
  auto logicalDevice() const -> LogicalDeviceSharedPtr {
    return logical_device_;
  }

 private:
  LogicalDeviceSharedPtr logical_device_{};
  DescriptorPoolSizeVec pool_sizes_{};
  Handle<VkDescriptorPool> descriptor_pool_{};
};
}  // namespace tupi
