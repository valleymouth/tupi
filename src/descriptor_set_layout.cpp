#include "tupi/descriptor_set_layout.h"

#include "tupi/descriptor_set_layout_binding.h"
#include "tupi/logical_device.h"

namespace tupi {
DescriptorSetLayout::~DescriptorSetLayout() {
  if (logical_device_) {
    vkDestroyDescriptorSetLayout(logical_device_->handle(),
                                 descriptor_set_layout_, nullptr);
  }
}

auto DescriptorSetLayout::handles(
    const DescriptorSetLayoutPtrVec& descriptor_set_layouts)
    -> std::vector<VkDescriptorSetLayout> {
  std::vector<VkDescriptorSetLayout> result;
  result.reserve(descriptor_set_layouts.size());
  for (const auto& descriptor_set_layout : descriptor_set_layouts) {
    result.emplace_back(descriptor_set_layout->handle());
  }
  return result;
}

DescriptorSetLayout::DescriptorSetLayout(LogicalDevicePtr logical_device,
                                         DescriptorSetLayoutBindingVec bindings)
    : logical_device_(std::move(logical_device)),
      bindings_(std::move(bindings)) {
  VkDescriptorSetLayoutCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  create_info.bindingCount = static_cast<uint32_t>(bindings_.size());
  auto vk_bindings = DescriptorSetLayoutBinding::handles(bindings_);
  create_info.pBindings = vk_bindings.data();

  if (vkCreateDescriptorSetLayout(logical_device_->handle(), &create_info,
                                  nullptr,
                                  &descriptor_set_layout_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout!");
  }
}

DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& other) {
  logical_device_ = std::move(other.logical_device_);
  other.logical_device_ = nullptr;
  bindings_ = std::move(other.bindings_);
  descriptor_set_layout_ = other.descriptor_set_layout_;
}

auto DescriptorSetLayout::operator=(DescriptorSetLayout&& other)
    -> DescriptorSetLayout& {
  logical_device_ = std::move(other.logical_device_);
  other.logical_device_ = nullptr;
  bindings_ = std::move(other.bindings_);
  descriptor_set_layout_ = other.descriptor_set_layout_;
  return *this;
}
}  // namespace tupi
