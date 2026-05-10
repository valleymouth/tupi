#ifndef TUPI_PIPELINE_VERTEX_INPUT_H
#define TUPI_PIPELINE_VERTEX_INPUT_H

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class PipelineVertexInput {
 public:
  PipelineVertexInput() = default;
  PipelineVertexInput(
      VertexInputBindingDescriptionVec binding_descriptions,
      VertexInputAttributeDescriptionVec attribute_descriptions);

  auto pipelineCreateInfo() const -> VkPipelineVertexInputStateCreateInfo {
    return pipeline_create_info_;
  }

 private:
  VkPipelineVertexInputStateCreateInfo pipeline_create_info_{};
  VertexInputBindingDescriptionVec binding_descriptions_{};
  VertexInputAttributeDescriptionVec attribute_descriptions_{};
};
}  // namespace tupi
#endif  // TUPI_PIPELINE_VERTEX_INPUT_H