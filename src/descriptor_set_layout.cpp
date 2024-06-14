#include "tupi/descriptor_set_layout.h"

#include "tupi/logical_device.h"

namespace tupi {
DescriptorSetLayout::~DescriptorSetLayout() {
  vkDestroyDescriptorSetLayout(logical_device_->handle(),
                               descriptor_set_layout_, nullptr);
}

auto DescriptorSetLayout::create(const LogicalDeviceSharedPtr& logical_device,
                                 const DescriptorSetLayoutBindingVec& bindings,
                                 VkDescriptorSetLayoutCreateInfo& create_info)
    -> VkDescriptorSetLayout {
  VkDescriptorSetLayout descriptor_set_layout;
  if (vkCreateDescriptorSetLayout(logical_device->handle(), &create_info,
                                  nullptr,
                                  &descriptor_set_layout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout!");
  }
  return descriptor_set_layout;
}

auto BindlessDescriptorSetLayout::create(
    const LogicalDeviceSharedPtr& logical_device,
    const DescriptorSetLayoutBindingVec& bindings,
    VkDescriptorSetLayoutCreateInfo& create_info) -> VkDescriptorSetLayout {
  create_info.flags =
      VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;

  VkDescriptorBindingFlags bindless_flags =
      VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
      VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT;

  auto binding_count = static_cast<uint32_t>(bindings.size());
  std::vector<VkDescriptorBindingFlags> binding_flags;
  for (auto i = 0; i < binding_count; i++) {
    binding_flags.push_back(bindless_flags);
  }

  VkDescriptorSetLayoutBindingFlagsCreateInfo binding_flags_create_info{
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
      nullptr};
  binding_flags_create_info.bindingCount = binding_count;
  binding_flags_create_info.pBindingFlags = binding_flags.data();
  create_info.pNext = &binding_flags_create_info;

  VkDescriptorSetLayout descriptor_set_layout;
  if (vkCreateDescriptorSetLayout(logical_device->handle(), &create_info,
                                  nullptr,
                                  &descriptor_set_layout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout!");
  }
  return descriptor_set_layout;
}
}  // namespace tupi
