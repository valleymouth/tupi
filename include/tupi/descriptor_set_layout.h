#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class DescriptorSetLayout {
 public:
  DescriptorSetLayout(LogicalDeviceSharedPtr logical_device,
                      DescriptorSetLayoutBindingVec bindings);
  ~DescriptorSetLayout();
  DescriptorSetLayout(const DescriptorSetLayout&) = delete;
  DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
  DescriptorSetLayout(DescriptorSetLayout&& other) = default;
  DescriptorSetLayout& operator=(DescriptorSetLayout&& other) = default;

  auto handle() const -> VkDescriptorSetLayout {
    return descriptor_set_layout_;
  }

 private:
  LogicalDeviceSharedPtr logical_device_{};
  DescriptorSetLayoutBindingVec bindings_{};
  Handle<VkDescriptorSetLayout> descriptor_set_layout_{};
};
}  // namespace tupi
