#include "tupi/pipeline_layout.h"

#include "tupi/descriptor_set_layout.h"
#include "tupi/logical_device.h"
#include "tupi/utility.h"

namespace tupi {
PipelineLayout::PipelineLayout(
    LogicalDeviceSharedPtr logical_device,
    DescriptorSetLayoutSharedPtrVec descriptor_set_layouts)
    : logical_device_(std::move(logical_device)) {
  VkPipelineLayoutCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  create_info.setLayoutCount =
      static_cast<uint32_t>(descriptor_set_layouts.size());
  auto vk_descriptor_set_layouts = handles(descriptor_set_layouts);
  create_info.pSetLayouts = vk_descriptor_set_layouts.data();
  create_info.pushConstantRangeCount = 0;     // Optional
  create_info.pPushConstantRanges = nullptr;  // Optional

  if (vkCreatePipelineLayout(logical_device_->handle(), &create_info, nullptr,
                             &pipeline_layout_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipeline layout!");
  }
}

PipelineLayout::~PipelineLayout() {
  vkDestroyPipelineLayout(logical_device_->handle(), pipeline_layout_, nullptr);
}
}  // namespace tupi
