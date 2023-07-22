#include "tupi/pipeline_dynamic_state.h"

namespace tupi {
PipelineDynamicState::PipelineDynamicState() {
  dynamic_states_ = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

  pipeline_create_info_.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  pipeline_create_info_.dynamicStateCount =
      static_cast<uint32_t>(dynamic_states_.size());
  pipeline_create_info_.pDynamicStates = dynamic_states_.data();
}
}  // namespace tupi
