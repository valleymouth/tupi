#ifndef TUPI_PIPELINE_VIEWPORT_STATE_H
#define TUPI_PIPELINE_VIEWPORT_STATE_H

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class PipelineViewportState {
 public:
  PipelineViewportState() = default;
  PipelineViewportState(VkViewport viewport, VkRect2D scissor);

  auto pipelineCreateInfo() const -> VkPipelineViewportStateCreateInfo {
    return pipeline_create_info_;
  }

 private:
  VkPipelineViewportStateCreateInfo pipeline_create_info_{};
};
}  // namespace tupi
#endif  // TUPI_PIPELINE_VIEWPORT_STATE_H