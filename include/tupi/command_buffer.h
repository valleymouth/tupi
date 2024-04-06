#pragma once

#include <functional>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class CommandBuffer {
 public:
  CommandBuffer(CommandPoolSharedPtr command_pool);
  ~CommandBuffer();
  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer& operator=(const CommandBuffer&) = delete;
  CommandBuffer(CommandBuffer&& other) = default;
  CommandBuffer& operator=(CommandBuffer&& other) = default;

  auto handle() const -> VkCommandBuffer { return command_buffer_; }
  auto beginRenderPass(FramebufferPtr framebuffer) -> void;
  auto endRenderPass() -> void;
  auto bindPipeline(PipelinePtr pipeline) -> void;
  auto bindDescriptorSets(PipelineLayoutPtr pipeline_layout,
                          DescriptorSetSharedPtrVec descriptor_sets) -> void;
  auto setViewport(VkViewport viewport) -> void;
  auto setScissor(VkRect2D rect) -> void;
  auto draw(BufferPtrVec vertex_buffers, OffsetVec offsets,
            uint32_t vertex_count, BufferSharedPtr index_buffer = {},
            Offset index_offset = 0, uint32_t index_count = 0) -> void;
  auto copy(BufferSharedPtr source, BufferSharedPtr destination,
            VkDeviceSize size, VkDeviceSize source_offset = 0,
            VkDeviceSize destination_offset = 0) -> void;
  auto copy(BufferSharedPtr source, Image2DPtr destination) -> void;
  auto reset() -> void {
    vkResetCommandBuffer(command_buffer_, 0);
    commands_.clear();
  }
  auto record(const FramebufferPtr& framebuffer, const PipelinePtr& pipeline)
      -> void;
  auto record(VkCommandBufferUsageFlags flags = 0) -> void;
  auto transitionImageLayout(IImagePtr image, VkFormat format,
                             VkImageLayout from, VkImageLayout to) -> void;

 private:
  using Command = std::function<void(CommandBuffer&)>;

  CommandPoolSharedPtr command_pool_{};
  Handle<VkCommandBuffer> command_buffer_{};
  std::vector<Command> commands_{};
};
}  // namespace tupi
