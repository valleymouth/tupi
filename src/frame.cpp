#include "tupi/frame.h"

#include "tupi/fence.h"
#include "tupi/framebuffer.h"
#include "tupi/logical_device.h"
#include "tupi/pipeline.h"
#include "tupi/queue.h"
#include "tupi/semaphore.h"
#include "tupi/surface.h"
#include "tupi/swapchain.h"

namespace tupi {
auto Frame::draw(SwapchainPtr& swapchain, FramebufferPtrVec& framebuffers,
                 const PipelinePtr& pipeline, const Queue& graphics_queue,
                 const Queue& present_queue,
                 DescriptorSetPtrVec descriptor_sets,
                 BufferPtrVec vertex_buffers, OffsetVec offsets,
                 uint32_t vertex_count, BufferPtr index_buffer,
                 Offset index_offset, uint32_t index_count) -> void {
  fence_->wait();
  auto acquired_result =
      swapchain->acquireNextImage(image_available_semaphore_);
  auto result = std::get<VkResult>(acquired_result);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    swapchain->logicalDevice()->waitIdle();
    swapchain->recreate();
    framebuffers = Framebuffer::enumerate(*swapchain, pipeline->renderPass());
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("Failed to acquire swap chain image!");
  }
  fence_->reset();
  auto image_index = std::get<uint32_t>(acquired_result);
  command_buffer_->reset();
  auto& framebuffer = framebuffers.at(image_index);
  command_buffer_->beginRenderPass(framebuffer);
  command_buffer_->bindPipeline(pipeline);
  command_buffer_->bindDescriptorSets(pipeline->pipelineLayout(),
                                      descriptor_sets);
  auto extent = framebuffer->extent();
  command_buffer_->setViewport({0.0f, 0.0f, static_cast<float>(extent.width),
                                static_cast<float>(extent.height), 0.0, 1.0});
  command_buffer_->setScissor({{0, 0}, extent});
  command_buffer_->draw(std::move(vertex_buffers), std::move(offsets),
                        vertex_count, std::move(index_buffer), index_offset,
                        index_count);
  command_buffer_->endRenderPass();
  command_buffer_->record();
  tupi::SemaphorePtrVec wait_semaphores = {image_available_semaphore_};
  tupi::SemaphorePtrVec signal_semaphores = {render_finished_semaphore_};
  graphics_queue.submit(command_buffer_, wait_semaphores,
                        {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
                        signal_semaphores, fence_);
  auto present_result =
      present_queue.present(swapchain, image_index, signal_semaphores);
  if (present_result == VK_ERROR_OUT_OF_DATE_KHR ||
      present_result == VK_SUBOPTIMAL_KHR) {
    swapchain->logicalDevice()->waitIdle();
    swapchain->recreate();
    framebuffers = Framebuffer::enumerate(*swapchain, pipeline->renderPass());
  } else if (present_result != VK_SUCCESS) {
    throw std::runtime_error("Failed to present swap chain image!");
  }
}

Frame::Frame(const LogicalDevicePtr& logical_device,
             const CommandPoolPtr& command_pool)
    : command_buffer_(CommandBuffer::create(command_pool)),
      image_available_semaphore_(std::move(Semaphore::create(logical_device))),
      render_finished_semaphore_(std::move(Semaphore::create(logical_device))),
      fence_(std::move(Fence::create(logical_device, true))) {}
}  // namespace tupi
