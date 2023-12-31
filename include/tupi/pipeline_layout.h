#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/resource.h"

namespace tupi {
class PipelineLayout : public internal::SharedResource<PipelineLayout> {
 public:
  PipelineLayout(Token, LogicalDevicePtr logical_device,
                 DescriptorSetLayoutPtrVec descriptor_sets);
  ~PipelineLayout();

  auto handle() const -> VkPipelineLayout;

 protected:
  PipelineLayout(const PipelineLayout&) = delete;
  PipelineLayout(PipelineLayout&& other);
  auto operator=(const PipelineLayout&) -> PipelineLayout& = delete;
  auto operator=(PipelineLayout&& other) -> PipelineLayout&;

 private:
  LogicalDevicePtr logical_device_{};
  VkPipelineLayout pipeline_layout_{VK_NULL_HANDLE};
};

inline auto PipelineLayout::handle() const -> VkPipelineLayout {
  return pipeline_layout_;
}
}  // namespace tupi
