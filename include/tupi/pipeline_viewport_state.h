#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class PipelineViewportState {
 public:
  PipelineViewportState() = default;
  PipelineViewportState(VkViewport viewport, VkRect2D scissor);

  auto pipelineCreateInfo() const -> VkPipelineViewportStateCreateInfo;

 private:
  VkPipelineViewportStateCreateInfo pipeline_create_info_{};
};

inline auto PipelineViewportState::pipelineCreateInfo() const
    -> VkPipelineViewportStateCreateInfo {
  return pipeline_create_info_;
}
}  // namespace tupi
