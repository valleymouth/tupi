#pragma once

#include <memory>

#include "tupi/descriptor_set_layout_binding.h"
#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"

namespace tupi {
class DescriptorSetLayout
    : public internal::Creatable<DescriptorSetLayout, std::shared_ptr> {
  friend class internal::Creatable<DescriptorSetLayout, std::shared_ptr>;

 public:
  ~DescriptorSetLayout();

  auto handle() const -> VkDescriptorSetLayout;

  static auto handles(const DescriptorSetLayoutPtrVec& descriptor_set_layouts)
      -> std::vector<VkDescriptorSetLayout>;

 protected:
  DescriptorSetLayout() = default;
  DescriptorSetLayout(LogicalDevicePtr logical_device,
                      DescriptorSetLayoutBindingVec bindings);
  DescriptorSetLayout(const DescriptorSetLayout&) = delete;
  DescriptorSetLayout(DescriptorSetLayout&& other);
  auto operator=(const DescriptorSetLayout&) -> DescriptorSetLayout& = delete;
  auto operator=(DescriptorSetLayout&& other) -> DescriptorSetLayout&;

 private:
  LogicalDevicePtr logical_device_{};
  DescriptorSetLayoutBindingVec bindings_{};
  VkDescriptorSetLayout descriptor_set_layout_{};
};

inline auto DescriptorSetLayout::handle() const -> VkDescriptorSetLayout {
  return descriptor_set_layout_;
}
}  // namespace tupi
