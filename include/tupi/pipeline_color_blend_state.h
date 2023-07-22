#pragma once

#include "tupi/fwd.h"

namespace tupi {
class PipelineColorBlendState {
 public:
  PipelineColorBlendState() = default;
  PipelineColorBlendState(PipelineColorBlendAttachmentStateVec attachments);

  auto pipelineCreateInfo() const -> VkPipelineColorBlendStateCreateInfo;

 private:
  VkPipelineColorBlendStateCreateInfo pipeline_create_info_{};
  PipelineColorBlendAttachmentStateVec attachments_{};
};

inline auto PipelineColorBlendState::pipelineCreateInfo() const
    -> VkPipelineColorBlendStateCreateInfo {
  return pipeline_create_info_;
}
}  // namespace tupi
