#pragma once

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