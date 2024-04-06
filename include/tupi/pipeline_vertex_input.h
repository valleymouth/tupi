#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class PipelineVertexInput {
 public:
  PipelineVertexInput() = default;

  auto pipelineCreateInfo() const -> VkPipelineVertexInputStateCreateInfo {
    return pipeline_create_info_;
  }

  template <typename T>
  static auto create() -> PipelineVertexInput {
    PipelineVertexInput result;
    result.binding_descriptions_ = T::bindingDescription();
    result.attribute_descriptions_ = T::attributeDescriptions();
    result.pipeline_create_info_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    result.pipeline_create_info_.vertexBindingDescriptionCount =
        static_cast<uint32_t>(result.binding_descriptions_.size());
    result.pipeline_create_info_.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(result.attribute_descriptions_.size());
    result.pipeline_create_info_.pVertexBindingDescriptions =
        result.binding_descriptions_.data();
    result.pipeline_create_info_.pVertexAttributeDescriptions =
        result.attribute_descriptions_.data();
    return result;
  }

 private:
  VkPipelineVertexInputStateCreateInfo pipeline_create_info_{};
  VertexInputBindingDescriptionVec binding_descriptions_{};
  VertexInputAttributeDescriptionVec attribute_descriptions_{};
};
}  // namespace tupi
