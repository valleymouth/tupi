#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class PipelineVertexInput {
 public:
  PipelineVertexInput();

  auto pipelineCreateInfo() const -> VkPipelineVertexInputStateCreateInfo;

 private:
  VkPipelineVertexInputStateCreateInfo pipeline_create_info_;
};

inline auto PipelineVertexInput::pipelineCreateInfo() const
    -> VkPipelineVertexInputStateCreateInfo {
  return pipeline_create_info_;
}
}  // namespace tupi
