#include "tupi/pipeline_multisample_state.h"

namespace tupi {
PipelineMultisampleState::PipelineMultisampleState() {
  pipeline_create_info_.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  pipeline_create_info_.sampleShadingEnable = VK_FALSE;
  pipeline_create_info_.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  pipeline_create_info_.minSampleShading = 1.0f;           // Optional
  pipeline_create_info_.pSampleMask = nullptr;             // Optional
  pipeline_create_info_.alphaToCoverageEnable = VK_FALSE;  // Optional
  pipeline_create_info_.alphaToOneEnable = VK_FALSE;       // Optional
}
}  // namespace tupi
