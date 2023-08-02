#include "tupi/descriptor_set_layout_binding.h"

namespace tupi {
DescriptorSetLayoutBinding::DescriptorSetLayoutBinding(
    uint32_t binding, VkDescriptorType type, uint32_t count,
    VkShaderStageFlags flags) {
  binding_.binding = binding;
  binding_.descriptorType = type;
  binding_.descriptorCount = count;  // for arrays or uniform blocks.
  binding_.stageFlags = flags;
  binding_.pImmutableSamplers = nullptr;  // Optional
}

auto DescriptorSetLayoutBinding::handles(
    const DescriptorSetLayoutBindingVec& bindings)
    -> std::vector<VkDescriptorSetLayoutBinding> {
  std::vector<VkDescriptorSetLayoutBinding> result;
  result.reserve(bindings.size());
  for (const auto& binding : bindings) {
    result.emplace_back(binding.handle());
  }
  return result;
}
}  // namespace tupi
