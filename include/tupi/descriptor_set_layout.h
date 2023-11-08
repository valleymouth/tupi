#pragma once

#include <memory>

#include "tupi/fwd.h"
#include "tupi/internal/resource.h"

namespace tupi {
class DescriptorSetLayout
    : public internal::SharedResource<DescriptorSetLayout> {
 public:
  DescriptorSetLayout(Token, LogicalDevicePtr logical_device,
                      DescriptorSetLayoutBindingVec bindings);
  ~DescriptorSetLayout();

  auto handle() const -> VkDescriptorSetLayout;

 protected:
  DescriptorSetLayout(const DescriptorSetLayout&) = delete;
  DescriptorSetLayout(DescriptorSetLayout&& other) = delete;
  auto operator=(const DescriptorSetLayout&) -> DescriptorSetLayout& = delete;
  auto operator=(DescriptorSetLayout&& other) -> DescriptorSetLayout& = delete;

 private:
  LogicalDevicePtr logical_device_{};
  DescriptorSetLayoutBindingVec bindings_{};
  VkDescriptorSetLayout descriptor_set_layout_{};
};

inline auto DescriptorSetLayout::handle() const -> VkDescriptorSetLayout {
  return descriptor_set_layout_;
}
}  // namespace tupi
