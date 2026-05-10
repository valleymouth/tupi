#ifndef TUPI_PIPELINE_DEPTH_STENCIL_STATE_H
#define TUPI_PIPELINE_DEPTH_STENCIL_STATE_H

#include <vulkan/vulkan.h>

#include "tupi/fwd.h"

namespace tupi {
class PipelineDepthStencilState {
 public:
  PipelineDepthStencilState();

  auto pipelineCreateInfo() const -> VkPipelineDepthStencilStateCreateInfo {
    return pipeline_create_info_;
  }

 private:
  VkPipelineDepthStencilStateCreateInfo pipeline_create_info_{};
};
}  // namespace tupi
#endif  // TUPI_PIPELINE_DEPTH_STENCIL_STATE_H