#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class CommandBuffer {
 public:
  CommandBuffer() = default;
  CommandBuffer(LogicalDevicePtr logical_device, CommandPoolPtr command_pool);
  ~CommandBuffer();

  auto handle() const -> VkCommandBuffer;
  auto beginRenderPass(FramebufferPtr framebuffer) -> void;
  auto endRenderPass() -> void;
  auto bindPipeline(PipelinePtr pipeline) -> void;
  auto setViewport(VkViewport viewport) -> void;
  auto setScissor(VkRect2D rect) -> void;
  auto draw(BufferPtrVec vertex_buffers, OffsetVec offsets,
            uint32_t vertex_count, BufferPtr index_buffer = {},
            Offset index_offset = 0, uint32_t index_count = 0) -> void;
  auto copy(BufferPtr source, BufferPtr destination, VkDeviceSize size,
            VkDeviceSize source_offset = 0, VkDeviceSize destination_offset = 0)
      -> void;
  auto reset() -> void;
  auto record(const FramebufferPtr& framebuffer, const PipelinePtr& pipeline)
      -> void;
  auto record(VkCommandBufferUsageFlags flags = 0) -> void;

 private:
  using Command = std::function<void(const CommandBuffer&)>;

  LogicalDevicePtr logical_device_{};
  CommandPoolPtr command_pool_{};
  VkCommandBuffer command_buffer_{VK_NULL_HANDLE};
  std::vector<Command> commands_{};
};

inline auto CommandBuffer::handle() const -> VkCommandBuffer {
  return command_buffer_;
}

inline auto CommandBuffer::reset() -> void {
  vkResetCommandBuffer(command_buffer_, 0);
  commands_.clear();
}
}  // namespace tupi
