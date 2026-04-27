#include "tupi/pipeline_vertex_input.h"

#include "tupi/shader.h"

namespace tupi {
PipelineVertexInput::PipelineVertexInput(
    VertexInputBindingDescriptionVec binding_descriptions,
    VertexInputAttributeDescriptionVec attribute_descriptions)
    : binding_descriptions_(std::move(binding_descriptions)),
      attribute_descriptions_(std::move(attribute_descriptions)) {
  pipeline_create_info_.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  pipeline_create_info_.vertexBindingDescriptionCount =
      static_cast<uint32_t>(binding_descriptions_.size());
  pipeline_create_info_.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(attribute_descriptions_.size());
  pipeline_create_info_.pVertexBindingDescriptions =
      binding_descriptions_.data();
  pipeline_create_info_.pVertexAttributeDescriptions =
      attribute_descriptions_.data();
}
}  // namespace tupi
