#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class PipelineLayout {
 public:
  PipelineLayout() = default;
  PipelineLayout(LogicalDevicePtr logical_device);
  PipelineLayout(const PipelineLayout&) = delete;
  PipelineLayout(PipelineLayout&& other);
  auto operator=(const PipelineLayout&) -> PipelineLayout& = delete;
  auto operator=(PipelineLayout&& other) -> PipelineLayout&;
  ~PipelineLayout();

  auto handle() const -> VkPipelineLayout;

 private:
  LogicalDevicePtr logical_device_{};
  VkPipelineLayout pipeline_layout_{VK_NULL_HANDLE};
};

inline auto PipelineLayout::handle() const -> VkPipelineLayout {
  return pipeline_layout_;
}
}  // namespace tupi
