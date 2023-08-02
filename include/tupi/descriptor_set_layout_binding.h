#pragma once

#include "tupi/fwd.h"

namespace tupi {
class DescriptorSetLayoutBinding {
 public:
  DescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType type,
                             uint32_t count, VkShaderStageFlags flags);

  auto handle() const -> VkDescriptorSetLayoutBinding;

  static auto handles(const DescriptorSetLayoutBindingVec& bindings)
      -> std::vector<VkDescriptorSetLayoutBinding>;

 private:
  VkDescriptorSetLayoutBinding binding_{};
};

inline auto DescriptorSetLayoutBinding::handle() const
    -> VkDescriptorSetLayoutBinding {
  return binding_;
}
}  // namespace tupi
