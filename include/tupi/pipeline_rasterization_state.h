#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class PipelineRasterizationState {
 public:
  PipelineRasterizationState();

  auto pipelineCreateInfo() const -> VkPipelineRasterizationStateCreateInfo;

 private:
  VkPipelineRasterizationStateCreateInfo pipeline_create_info_{};
};

inline auto PipelineRasterizationState::pipelineCreateInfo() const
    -> VkPipelineRasterizationStateCreateInfo {
  return pipeline_create_info_;
}
}  // namespace tupi
