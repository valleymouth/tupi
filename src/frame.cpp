#include "tupi/frame.h"

#include "tupi/fence.h"
#include "tupi/framebuffer.h"
#include "tupi/logical_device.h"
#include "tupi/pipeline.h"
#include "tupi/queue.h"
#include "tupi/semaphore.h"
#include "tupi/surface.h"
#include "tupi/swapchain.h"
#include "tupi/tick.h"

namespace tupi {
Frame::Frame(const LogicalDeviceSharedPtr& logical_device,
             const CommandPoolSharedPtr& command_pool)
    : command_buffer_(std::make_unique<CommandBuffer>(std::move(command_pool))),
      image_available_semaphore_(std::make_shared<Semaphore>(logical_device)),
      render_finished_semaphore_(std::make_shared<Semaphore>(logical_device)),
      fence_(std::make_shared<Fence>(std::move(logical_device), true)),
      last_frame_time_point_(Clock::now()) {}

auto Frame::draw(SwapchainSharedPtr& swapchain,
                 FramebufferSharedPtrVec& framebuffers,
                 const PipelineSharedPtr& pipeline, const Queue& graphics_queue,
                 const Queue& present_queue,
                 DescriptorSetSharedPtrVec descriptor_sets,
                 BufferSharedPtrVec vertex_buffers, OffsetVec offsets,
                 uint32_t vertex_count, BufferSharedPtr index_buffer,
                 Offset index_offset, uint32_t index_count) -> void {
  auto now = Clock::now();
  float delta_time_in_seconds =
      std::chrono::duration<float, std::chrono::seconds::period>(
          now - last_frame_time_point_)
          .count();
  last_frame_time_point_ = now;

  for (const auto& tick_observer : tick_observers_) {
    tick_observer->tick(delta_time_in_seconds);
  }

  fence_->wait();
  auto acquired_result =
      swapchain->acquireNextImage(image_available_semaphore_);
  auto result = std::get<VkResult>(acquired_result);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    swapchain->logicalDevice()->waitIdle();
    swapchain->recreate();
    framebuffers = Framebuffer::enumerate(swapchain, pipeline->renderPass());
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
  tupi::SemaphoreSharedPtrVec wait_semaphores = {image_available_semaphore_};
  tupi::SemaphoreSharedPtrVec signal_semaphores = {render_finished_semaphore_};
  graphics_queue.submit(command_buffer_, wait_semaphores,
                        {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
                        signal_semaphores, fence_);
  auto present_result =
      present_queue.present(swapchain, image_index, signal_semaphores);
  if (present_result == VK_ERROR_OUT_OF_DATE_KHR ||
      present_result == VK_SUBOPTIMAL_KHR) {
    swapchain->logicalDevice()->waitIdle();
    swapchain->recreate();
    framebuffers = Framebuffer::enumerate(swapchain, pipeline->renderPass());
  } else if (present_result != VK_SUCCESS) {
    throw std::runtime_error("Failed to present swap chain image!");
  }
}

auto Frame::addTickObserver(ITickObserverSharedPtr tick_observer) -> void {
  tick_observers_.push_back(std::move(tick_observer));
}
}  // namespace tupi
