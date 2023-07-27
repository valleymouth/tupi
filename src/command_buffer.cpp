#include "tupi/command_buffer.h"

#include "tupi/buffer.h"
#include "tupi/command_pool.h"
#include "tupi/framebuffer.h"
#include "tupi/logical_device.h"
#include "tupi/pipeline.h"
#include "tupi/render_pass.h"

namespace tupi {
CommandBuffer::CommandBuffer(LogicalDevicePtr logical_device,
                             CommandPoolPtr command_pool)
    : logical_device_(std::move(logical_device)),
      command_pool_(std::move(command_pool)) {
  VkCommandBufferAllocateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  create_info.commandPool = command_pool_->handle();
  create_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  create_info.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(logical_device_->handle(), &create_info,
                               &command_buffer_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate command buffers!");
  }
}

auto CommandBuffer::beginRenderPass(FramebufferPtr framebuffer) -> void {
  commands_.emplace_back([framebuffer_ = std::move(framebuffer)](
                             const CommandBuffer& command_buffer) {
    auto extent = framebuffer_->extent();
    VkRenderPassBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    begin_info.renderPass = framebuffer_->renderPass()->handle();
    begin_info.framebuffer = framebuffer_->handle();
    begin_info.renderArea.offset = {0, 0};
    begin_info.renderArea.extent = extent;
    VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    begin_info.clearValueCount = 1;
    begin_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(command_buffer.handle(), &begin_info,
                         VK_SUBPASS_CONTENTS_INLINE);
  });
}

CommandBuffer::~CommandBuffer() {
  vkFreeCommandBuffers(logical_device_->handle(), command_pool_->handle(), 1,
                       &command_buffer_);
}

auto CommandBuffer::endRenderPass() -> void {
  commands_.emplace_back([](const CommandBuffer& command_buffer) {
    vkCmdEndRenderPass(command_buffer.handle());
  });
}

auto CommandBuffer::bindPipeline(PipelinePtr pipeline) -> void {
  commands_.emplace_back(
      [pipeline_ = std::move(pipeline)](const CommandBuffer& command_buffer) {
        vkCmdBindPipeline(command_buffer.handle(),
                          VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->handle());
      });
}

auto CommandBuffer::setViewport(VkViewport viewport) -> void {
  commands_.emplace_back([=](const CommandBuffer& command_buffer) {
    vkCmdSetViewport(command_buffer.handle(), 0, 1, &viewport);
  });
}

auto CommandBuffer::setScissor(VkRect2D rect) -> void {
  commands_.emplace_back([=](const CommandBuffer& command_buffer) {
    vkCmdSetScissor(command_buffer.handle(), 0, 1, &rect);
  });
}

auto CommandBuffer::draw(BufferPtrVec vertex_buffers, OffsetVec offsets,
                         uint32_t vertex_count) -> void {
  commands_.emplace_back(
      [=, vertex_buffers_ = std::move(vertex_buffers),
       offsets_ = std::move(offsets)](const CommandBuffer& command_buffer) {
        std::vector<VkBuffer> vk_vertex_buffers;
        vk_vertex_buffers.reserve(vertex_buffers_.size());
        for (const auto& vertex_buffer : vertex_buffers_) {
          vk_vertex_buffers.emplace_back(vertex_buffer->handle());
        }
        vkCmdBindVertexBuffers(command_buffer.handle(), 0, 1,
                               vk_vertex_buffers.data(), offsets_.data());
        vkCmdDraw(command_buffer.handle(), vertex_count, 1, 0, 0);
      });
}

auto CommandBuffer::copy(BufferPtr source, BufferPtr destination,
                         VkDeviceSize size, VkDeviceSize source_offset,
                         VkDeviceSize destination_offset) -> void {
  commands_.emplace_back(
      [=, source_ = std::move(source), destination_ = std::move(destination)](
          const CommandBuffer& command_buffer) {
        VkBufferCopy buffer_copy{};
        buffer_copy.srcOffset = source_offset;
        buffer_copy.dstOffset = destination_offset;
        buffer_copy.size = size;
        vkCmdCopyBuffer(command_buffer.handle(), source_->handle(),
                        destination_->handle(), 1, &buffer_copy);
      });
}

auto CommandBuffer::record(const FramebufferPtr& framebuffer,
                           const PipelinePtr& pipeline) -> void {
  VkCommandBufferBeginInfo begin_info{};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = 0;                   // Optional
  begin_info.pInheritanceInfo = nullptr;  // Optional

  if (vkBeginCommandBuffer(command_buffer_, &begin_info) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin recording command buffer!");
  }

  auto extent = framebuffer->extent();
  VkRenderPassBeginInfo render_pass_begin_info{};
  render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_begin_info.renderPass = framebuffer->renderPass()->handle();
  render_pass_begin_info.framebuffer = framebuffer->handle();
  render_pass_begin_info.renderArea.offset = {0, 0};
  render_pass_begin_info.renderArea.extent = extent;
  VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  render_pass_begin_info.clearValueCount = 1;
  render_pass_begin_info.pClearValues = &clear_color;

  vkCmdBeginRenderPass(command_buffer_, &render_pass_begin_info,
                       VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipeline->handle());

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(extent.width);
  viewport.height = static_cast<float>(extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(command_buffer_, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = extent;
  vkCmdSetScissor(command_buffer_, 0, 1, &scissor);

  for (const auto& command : commands_) {
    command(*this);
  }

  vkCmdEndRenderPass(command_buffer_);

  if (vkEndCommandBuffer(command_buffer_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to record command buffer!");
  }
}

auto CommandBuffer::record(VkCommandBufferUsageFlags flags) -> void {
  VkCommandBufferBeginInfo begin_info{};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = flags;

  if (vkBeginCommandBuffer(command_buffer_, &begin_info) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin recording command buffer!");
  }

  for (const auto& command : commands_) {
    command(*this);
  }

  if (vkEndCommandBuffer(command_buffer_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to record command buffer!");
  }
}
}  // namespace tupi
