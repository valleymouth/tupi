#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class PipelineInputAssembly {
 public:
  PipelineInputAssembly();

  auto pipelineCreateInfo() const -> VkPipelineInputAssemblyStateCreateInfo {
    return pipeline_create_info_;
  }

 private:
  VkPipelineInputAssemblyStateCreateInfo pipeline_create_info_{};
};
}  // namespace tupi
