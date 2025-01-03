#pragma once

#include <functional>
#include <map>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
struct Command {
  using Index = uint32_t;
  using Callable = std::function<void(CommandBuffer&)>;

  auto operator()(CommandBuffer& command_buffer) const -> void {
    callable(command_buffer);
  }

  auto operator<(const Command& other) const -> bool {
    return index < other.index;
  }

  Index index{0};
  Callable callable{};
};

class CommandBuffer {
 public:
  CommandBuffer(CommandPoolSharedPtr command_pool);
  ~CommandBuffer();
  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer& operator=(const CommandBuffer&) = delete;
  CommandBuffer(CommandBuffer&& other) = default;
  CommandBuffer& operator=(CommandBuffer&& other) = default;

  auto handle() const -> VkCommandBuffer { return command_buffer_; }
  auto add(const CommandVec& commands) -> void;
  auto beginRenderPass(FramebufferSharedPtr framebuffer) -> void;
  auto endRenderPass() -> void;
  auto bindPipeline(PipelineSharedPtr pipeline) -> void;
  auto bindDescriptorSets(PipelineLayoutSharedPtr pipeline_layout,
                          DescriptorSetSharedPtrVec descriptor_sets) -> void;
  auto setViewport(VkViewport viewport) -> void;
  auto setScissor(VkRect2D rect) -> void;
  auto draw(BufferSharedPtrVec vertex_buffers, OffsetVec offsets,
            uint32_t vertex_count, BufferSharedPtr index_buffer = {},
            Offset index_offset = 0, uint32_t index_count = 0) -> void;
  auto copy(BufferSharedPtr source, BufferSharedPtr destination,
            VkDeviceSize size, VkDeviceSize source_offset = 0,
            VkDeviceSize destination_offset = 0) -> void;
  auto copy(BufferSharedPtr source, Image2DSharedPtr destination) -> void;
  auto reset() -> void {
    vkResetCommandBuffer(command_buffer_, 0);
    commands_.clear();
  }
  auto record(const FramebufferSharedPtr& framebuffer,
              const PipelineSharedPtr& pipeline) -> void;
  auto record(VkCommandBufferUsageFlags flags = 0) -> void;
  auto transitionImageLayout(IImageSharedPtr image, VkFormat format,
                             VkImageLayout from, VkImageLayout to) -> void;

 private:
  CommandPoolSharedPtr command_pool_{};
  Handle<VkCommandBuffer> command_buffer_{};
  std::vector<Command> commands_{};
};
}  // namespace tupi
