#include "tupi/pipeline_rasterization_state.h"

namespace tupi {
PipelineRasterizationState::PipelineRasterizationState() {
  pipeline_create_info_.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  pipeline_create_info_.depthClampEnable = VK_FALSE;
  pipeline_create_info_.rasterizerDiscardEnable = VK_FALSE;
  pipeline_create_info_.polygonMode = VK_POLYGON_MODE_FILL;
  pipeline_create_info_.lineWidth = 1.0f;
  pipeline_create_info_.cullMode = VK_CULL_MODE_BACK_BIT;
  pipeline_create_info_.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  pipeline_create_info_.depthBiasEnable = VK_FALSE;
  pipeline_create_info_.depthBiasConstantFactor = 0.0f;  // Optional
  pipeline_create_info_.depthBiasClamp = 0.0f;           // Optional
  pipeline_create_info_.depthBiasSlopeFactor = 0.0f;     // Optional
}
}  // namespace tupi
