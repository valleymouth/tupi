#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class PipelineLayout {
 public:
  PipelineLayout(LogicalDeviceSharedPtr logical_device,
                 DescriptorSetLayoutSharedPtrVec descriptor_sets);
  ~PipelineLayout();
  PipelineLayout(const PipelineLayout&) = delete;
  auto operator=(const PipelineLayout&) -> PipelineLayout& = delete;
  PipelineLayout(PipelineLayout&& other) = default;
  auto operator=(PipelineLayout&& other) -> PipelineLayout& = default;

  auto handle() const -> VkPipelineLayout { return pipeline_layout_; }

 private:
  LogicalDeviceSharedPtr logical_device_{};
  Handle<VkPipelineLayout> pipeline_layout_{};
};
}  // namespace tupi
