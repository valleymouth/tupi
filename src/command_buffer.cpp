#include "tupi/command_buffer.h"

#include "tupi/buffer.h"
#include "tupi/command_pool.h"
#include "tupi/descriptor_set.h"
#include "tupi/framebuffer.h"
#include "tupi/image_2d.h"
#include "tupi/logical_device.h"
#include "tupi/pipeline.h"
#include "tupi/render_pass.h"

namespace tupi {
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
    std::array<VkClearValue, 2> clear_values;
    clear_values[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
    clear_values[1].depthStencil = {1.0f, 0};
    begin_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
    begin_info.pClearValues = clear_values.data();

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

auto CommandBuffer::bindDescriptorSets(PipelineLayoutPtr pipeline_layout,
                                       DescriptorSetPtrVec descriptor_sets)
    -> void {
  commands_.emplace_back([pipeline_layout_ = std::move(pipeline_layout),
                          descriptor_sets_ = std::move(descriptor_sets)](
                             const CommandBuffer& command_buffer) {
    auto vk_descriptor_sets = DescriptorSet::handles(descriptor_sets_);
    vkCmdBindDescriptorSets(command_buffer.handle(),
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            pipeline_layout_->handle(), 0,
                            static_cast<uint32_t>(vk_descriptor_sets.size()),
                            vk_descriptor_sets.data(), 0, nullptr);
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

auto CommandBuffer::draw(BufferPtrVec vertex_buffers, OffsetVec vertex_offsets,
                         uint32_t vertex_count, BufferPtr index_buffer,
                         Offset index_offset, uint32_t index_count) -> void {
  commands_.emplace_back([=, vertex_buffers_ = std::move(vertex_buffers),
                          vertex_offsets_ = std::move(vertex_offsets),
                          index_buffer_ = std::move(index_buffer)](
                             const CommandBuffer& command_buffer) {
    auto vk_vertex_buffers = Buffer::handles(vertex_buffers_);
    vkCmdBindVertexBuffers(command_buffer.handle(), 0, 1,
                           vk_vertex_buffers.data(), vertex_offsets_.data());
    if (index_buffer_) {
      vkCmdBindIndexBuffer(command_buffer.handle(), index_buffer_->handle(),
                           index_offset, VK_INDEX_TYPE_UINT16);
      vkCmdDrawIndexed(command_buffer.handle(), index_count, 1, 0, 0, 0);
    } else {
      vkCmdDraw(command_buffer.handle(), vertex_count, 1, 0, 0);
    }
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

auto CommandBuffer::copy(BufferPtr source, Image2DPtr destination) -> void {
  commands_.emplace_back([=, source_ = std::move(source),
                          destination_ = std::move(destination)](
                             const CommandBuffer& command_buffer) {
    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {destination_->width(), destination_->height(), 1};

    vkCmdCopyBufferToImage(command_buffer.handle(), source_->handle(),
                           destination_->handle(),
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
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

auto CommandBuffer::transitionImageLayout(ImagePtr image, VkFormat format,
                                          VkImageLayout from, VkImageLayout to)
    -> void {
  commands_.emplace_back(
      [=, image_ = std::move(image)](const CommandBuffer& command_buffer) {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = from;
        barrier.newLayout = to;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image_->handle();
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags source_stage;
        VkPipelineStageFlags destination_stage;

        if (from == VK_IMAGE_LAYOUT_UNDEFINED &&
            to == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
          barrier.srcAccessMask = 0;
          barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
          source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
          destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (from == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                   to == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
          barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
          barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
          source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
          destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
          throw std::invalid_argument("Unsupported layout transition!");
        }

        vkCmdPipelineBarrier(command_buffer.handle(), source_stage,
                             destination_stage, 0, 0, nullptr, 0, nullptr, 1,
                             &barrier);
      });
}

CommandBuffer::CommandBuffer(CommandPoolPtr command_pool)
    : logical_device_(command_pool->logicalDevice()),
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
}  // namespace tupi
