#include "tupi/command_buffer.h"

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

auto CommandBuffer::record(const FramebufferPtr& framebuffer,
                           const PipelinePtr& pipeline) -> void {
  VkCommandBufferBeginInfo command_buffer_begin_info{};
  command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  command_buffer_begin_info.flags = 0;                   // Optional
  command_buffer_begin_info.pInheritanceInfo = nullptr;  // Optional

  if (vkBeginCommandBuffer(command_buffer_, &command_buffer_begin_info) !=
      VK_SUCCESS) {
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

  vkCmdDraw(command_buffer_, 3, 1, 0, 0);

  vkCmdEndRenderPass(command_buffer_);

  if (vkEndCommandBuffer(command_buffer_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to record command buffer!");
  }
}
}  // namespace tupi
