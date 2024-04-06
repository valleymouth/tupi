#include "tupi/descriptor_set_layout.h"

#include "tupi/logical_device.h"

namespace tupi {
DescriptorSetLayout::DescriptorSetLayout(LogicalDeviceSharedPtr logical_device,
                                         DescriptorSetLayoutBindingVec bindings)
    : logical_device_(std::move(logical_device)),
      bindings_(std::move(bindings)) {
  VkDescriptorSetLayoutCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  create_info.bindingCount = static_cast<uint32_t>(bindings_.size());
  create_info.pBindings = bindings_.data();

  if (vkCreateDescriptorSetLayout(logical_device_->handle(), &create_info,
                                  nullptr,
                                  &descriptor_set_layout_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout!");
  }
}

DescriptorSetLayout::~DescriptorSetLayout() {
  vkDestroyDescriptorSetLayout(logical_device_->handle(),
                               descriptor_set_layout_, nullptr);
}
}  // namespace tupi
