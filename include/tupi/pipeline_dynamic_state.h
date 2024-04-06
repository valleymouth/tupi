#pragma once

#include "tupi/fwd.h"

namespace tupi {
class PipelineDynamicState {
 public:
  PipelineDynamicState();

  auto pipelineCreateInfo() const -> VkPipelineDynamicStateCreateInfo {
    return pipeline_create_info_;
  }

 private:
  DynamicStateVec dynamic_states_{};
  VkPipelineDynamicStateCreateInfo pipeline_create_info_{};
};
}  // namespace tupi
