#include "tupi/pipeline_vertex_input.h"

namespace tupi {
PipelineVertexInput::PipelineVertexInput() {
  pipeline_create_info_.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  pipeline_create_info_.pNext = nullptr;
  pipeline_create_info_.flags = 0;
  pipeline_create_info_.vertexBindingDescriptionCount = 0;
  pipeline_create_info_.pVertexBindingDescriptions = nullptr;  // Optional
  pipeline_create_info_.vertexAttributeDescriptionCount = 0;
  pipeline_create_info_.pVertexAttributeDescriptions = nullptr;  // Optional
}
}  // namespace tupi
