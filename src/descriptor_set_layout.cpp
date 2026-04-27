#include "tupi/descriptor_set_layout.h"

#include "tupi/logical_device.h"

namespace tupi {
auto DescriptorSetLayoutInfo::merge(const DescriptorSetLayoutInfo& other)
    -> void {
  if (bindless != other.bindless) {
    throw std::runtime_error("Can't merge DescriptorSetLayoutInfo!");
  }

  DescriptorSetLayoutBindingVec to_append;
  for (auto& other_binding : other.bindings) {
    bool found = false;
    for (auto& binding : bindings) {
      if (binding.binding == other_binding.binding) {
        found = true;
        if (binding.descriptorCount != other_binding.descriptorCount ||
            binding.descriptorType != other_binding.descriptorType ||
            binding.pImmutableSamplers != other_binding.pImmutableSamplers) {
          throw std::runtime_error("Can't merge DescriptorSetLayoutInfo!");
        }
        binding.stageFlags |= other_binding.stageFlags;
      }
    }
    if (!found) {
      to_append.push_back(other_binding);
    }
  }
  bindings.append_range(std::move(to_append));
}

VkDescriptorSetLayout DescriptorSetLayoutConstructor::construct(
    const LogicalDeviceSharedPtr& logical_device,
    const DescriptorSetLayoutBindingVec& bindings) const {
  VkDescriptorSetLayoutCreateInfo create_info{
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
  auto binding_count = static_cast<uint32_t>(bindings.size());
  create_info.bindingCount = binding_count;
  create_info.pBindings = bindings.empty() ? nullptr : bindings.data();

  VkDescriptorSetLayout descriptor_set_layout;
  if (vkCreateDescriptorSetLayout(logical_device->handle(), &create_info,
                                  nullptr,
                                  &descriptor_set_layout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout!");
  }
  return descriptor_set_layout;
}

VkDescriptorSetLayout DescriptorSetLayoutBindlessConstructor::construct(
    const LogicalDeviceSharedPtr& logical_device,
    const DescriptorSetLayoutBindingVec& bindings) const {
  VkDescriptorSetLayoutCreateInfo create_info{
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
  auto binding_count = static_cast<uint32_t>(bindings.size());
  create_info.bindingCount = binding_count;
  create_info.pBindings = bindings.data();

  create_info.flags =
      VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;

  VkDescriptorBindingFlags bindless_flags =
      VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
      VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT;

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

DescriptorSetLayout::DescriptorSetLayout(LogicalDeviceSharedPtr logical_device)
    : logical_device_(std::move(logical_device)) {
  DescriptorSetLayoutConstructor constructor{};
  descriptor_set_layout_ = constructor.construct(logical_device_, {});
}

DescriptorSetLayout::DescriptorSetLayout(
    LogicalDeviceSharedPtr logical_device,
    DescriptorSetLayoutBindingVec bindings,
    const DescriptorSetLayoutConstructor& constructor)
    : logical_device_(std::move(logical_device)),
      bindings_(std::move(bindings)) {
  descriptor_set_layout_ = constructor.construct(logical_device_, bindings_);
}

DescriptorSetLayout::~DescriptorSetLayout() {
  vkDestroyDescriptorSetLayout(logical_device_->handle(),
                               descriptor_set_layout_, nullptr);
}
}  // namespace tupi
