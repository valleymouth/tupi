#include "tupi/pipeline_depth_stencil_state.h"

namespace tupi {
PipelineDepthStencilState::PipelineDepthStencilState() {
  pipeline_create_info_.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  pipeline_create_info_.depthTestEnable = VK_TRUE;
  pipeline_create_info_.depthWriteEnable = VK_TRUE;
  pipeline_create_info_.depthCompareOp = VK_COMPARE_OP_LESS;
  pipeline_create_info_.depthBoundsTestEnable = VK_FALSE;
  pipeline_create_info_.minDepthBounds = 0.0f;  // Optional
  pipeline_create_info_.maxDepthBounds = 1.0f;  // Optional
  pipeline_create_info_.stencilTestEnable = VK_FALSE;
  pipeline_create_info_.front = {};  // Optional
  pipeline_create_info_.back = {};   // Optional
}
}  // namespace tupi