#ifndef TUPI_PIPELINE_MULTISAMPLE_STATE_H
#define TUPI_PIPELINE_MULTISAMPLE_STATE_H

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class PipelineMultisampleState {
 public:
  PipelineMultisampleState();

  auto pipelineCreateInfo() const -> VkPipelineMultisampleStateCreateInfo {
    return pipeline_create_info_;
  }

 private:
  VkPipelineMultisampleStateCreateInfo pipeline_create_info_{};
};
}  // namespace tupi
#endif  // TUPI_PIPELINE_MULTISAMPLE_STATE_H