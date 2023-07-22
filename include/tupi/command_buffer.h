#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class CommandBuffer {
 public:
  CommandBuffer() = default;
  CommandBuffer(LogicalDevicePtr logical_device, CommandPoolPtr command_pool);

  auto handle() const -> VkCommandBuffer;
  auto reset() const -> void;
  auto record(const FramebufferPtr& framebuffer, const PipelinePtr& pipeline)
      -> void;

 private:
  LogicalDevicePtr logical_device_{};
  CommandPoolPtr command_pool_{};
  VkCommandBuffer command_buffer_{VK_NULL_HANDLE};
};

inline auto CommandBuffer::handle() const -> VkCommandBuffer {
  return command_buffer_;
}

inline auto CommandBuffer::reset() const -> void {
  vkResetCommandBuffer(command_buffer_, 0);
}
}  // namespace tupi
