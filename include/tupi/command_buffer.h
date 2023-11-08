#pragma once

#include <functional>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/resource.h"

namespace tupi {
class CommandBuffer : public internal::UniqueResource<CommandBuffer> {
 public:
  CommandBuffer(Token, CommandPoolPtr command_pool);
  ~CommandBuffer();

  auto handle() const -> VkCommandBuffer;
  auto beginRenderPass(FramebufferPtr framebuffer) -> void;
  auto endRenderPass() -> void;
  auto bindPipeline(PipelinePtr pipeline) -> void;
  auto bindDescriptorSets(PipelineLayoutPtr pipeline_layout,
                          DescriptorSetPtrVec descriptor_sets) -> void;
  auto setViewport(VkViewport viewport) -> void;
  auto setScissor(VkRect2D rect) -> void;
  auto draw(BufferPtrVec vertex_buffers, OffsetVec offsets,
            uint32_t vertex_count, BufferPtr index_buffer = {},
            Offset index_offset = 0, uint32_t index_count = 0) -> void;
  auto copy(BufferPtr source, BufferPtr destination, VkDeviceSize size,
            VkDeviceSize source_offset = 0, VkDeviceSize destination_offset = 0)
      -> void;
  auto copy(BufferPtr source, Image2DPtr destination) -> void;
  auto reset() -> void;
  auto record(const FramebufferPtr& framebuffer, const PipelinePtr& pipeline)
      -> void;
  auto record(VkCommandBufferUsageFlags flags = 0) -> void;
  auto transitionImageLayout(ImagePtr image, VkFormat format,
                             VkImageLayout from, VkImageLayout to) -> void;

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
