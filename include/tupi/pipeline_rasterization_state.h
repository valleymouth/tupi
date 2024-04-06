#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class PipelineRasterizationState {
 public:
  PipelineRasterizationState();

  auto pipelineCreateInfo() const -> VkPipelineRasterizationStateCreateInfo {
    return pipeline_create_info_;
  }

 private:
  VkPipelineRasterizationStateCreateInfo pipeline_create_info_{};
};
}  // namespace tupi
