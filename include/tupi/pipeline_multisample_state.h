#pragma once

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
