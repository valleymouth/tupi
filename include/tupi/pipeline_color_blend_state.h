#ifndef TUPI_PIPELINE_COLOR_BLEND_STATE_H
#define TUPI_PIPELINE_COLOR_BLEND_STATE_H

#include "tupi/fwd.h"

namespace tupi {
class PipelineColorBlendState {
 public:
  PipelineColorBlendState();
  PipelineColorBlendState(PipelineColorBlendAttachmentStateVec attachments);

  auto pipelineCreateInfo() const -> VkPipelineColorBlendStateCreateInfo {
    return pipeline_create_info_;
  }

 private:
  VkPipelineColorBlendStateCreateInfo pipeline_create_info_{};
  PipelineColorBlendAttachmentStateVec attachments_{};
};
}  // namespace tupi
#endif  // TUPI_PIPELINE_COLOR_BLEND_STATE_H