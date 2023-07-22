#include "tupi/pipeline_color_blend_state.h"

namespace tupi {
PipelineColorBlendState::PipelineColorBlendState(
    PipelineColorBlendAttachmentStateVec attachments)
    : attachments_(std::move(attachments)) {
  pipeline_create_info_.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  pipeline_create_info_.logicOpEnable = VK_FALSE;
  pipeline_create_info_.logicOp = VK_LOGIC_OP_COPY;  // Optional
  pipeline_create_info_.attachmentCount = attachments_.size();
  pipeline_create_info_.pAttachments = attachments_.data();
  pipeline_create_info_.blendConstants[0] = 0.0f;  // Optional
  pipeline_create_info_.blendConstants[1] = 0.0f;  // Optional
  pipeline_create_info_.blendConstants[2] = 0.0f;  // Optional
  pipeline_create_info_.blendConstants[3] = 0.0f;  // Optional
}
}  // namespace tupi
