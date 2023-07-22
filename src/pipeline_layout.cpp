#include "tupi/pipeline_layout.h"

#include "tupi/logical_device.h"

namespace tupi {
PipelineLayout::~PipelineLayout() {
  if (logical_device_) {
    vkDestroyPipelineLayout(logical_device_->handle(), pipeline_layout_,
                            nullptr);
  }
}

PipelineLayout::PipelineLayout(LogicalDevicePtr logical_device)
    : logical_device_(std::move(logical_device)) {
  VkPipelineLayoutCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  create_info.setLayoutCount = 0;             // Optional
  create_info.pSetLayouts = nullptr;          // Optional
  create_info.pushConstantRangeCount = 0;     // Optional
  create_info.pPushConstantRanges = nullptr;  // Optional

  if (vkCreatePipelineLayout(logical_device_->handle(), &create_info, nullptr,
                             &pipeline_layout_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipeline layout!");
  }
}

PipelineLayout::PipelineLayout(PipelineLayout&& other) {
  logical_device_ = std::move(other.logical_device_);
  other.logical_device_ = nullptr;
  pipeline_layout_ = other.pipeline_layout_;
  other.pipeline_layout_ = VK_NULL_HANDLE;
}

auto PipelineLayout::operator=(PipelineLayout&& other) -> PipelineLayout& {
  logical_device_ = std::move(other.logical_device_);
  other.logical_device_ = nullptr;
  pipeline_layout_ = other.pipeline_layout_;
  other.pipeline_layout_ = VK_NULL_HANDLE;
  return *this;
}
}  // namespace tupi
