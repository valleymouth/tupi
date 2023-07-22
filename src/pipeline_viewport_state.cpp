#include "tupi/pipeline_viewport_state.h"

namespace tupi {
PipelineViewportState::PipelineViewportState(VkViewport viewport,
                                             VkRect2D scissor) {
  pipeline_create_info_.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  pipeline_create_info_.viewportCount = 1;
  pipeline_create_info_.pViewports = &viewport;
  pipeline_create_info_.scissorCount = 1;
  pipeline_create_info_.pScissors = &scissor;
}
}  // namespace tupi
