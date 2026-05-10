#ifndef TUPI_PIPELINE_RASTERIZATION_STATE_H
#define TUPI_PIPELINE_RASTERIZATION_STATE_H

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
#endif  // TUPI_PIPELINE_RASTERIZATION_STATE_H