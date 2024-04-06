#include "tupi/queue.h"

#include "tupi/command_buffer.h"
#include "tupi/fence.h"
#include "tupi/logical_device.h"
#include "tupi/queue_family.h"
#include "tupi/semaphore.h"
#include "tupi/swapchain.h"

namespace tupi {
Queue::Queue(LogicalDeviceSharedPtr device, const QueueFamily& queue_family,
             uint32_t queue_index)
    : logical_device_(std::move(device)) {
  vkGetDeviceQueue(logical_device_->handle(), queue_family.index(), queue_index,
                   &queue_);
}

auto Queue::submit(const CommandBufferPtr& command_buffer,
                   const SemaphorePtrVec& wait_semaphores,
                   const PipelineStageFlagsVec& wait_stages,
                   const SemaphorePtrVec& signal_semaphores,
                   const FenceSharedPtr& fence) const -> void {
  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  auto vk_wait_semaphores = Semaphore::handles(wait_semaphores);
  submit_info.waitSemaphoreCount = vk_wait_semaphores.size();
  submit_info.pWaitSemaphores = vk_wait_semaphores.data();
  submit_info.pWaitDstStageMask = wait_stages.data();
  auto vk_command_buffer = command_buffer->handle();
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &vk_command_buffer;
  auto vk_signal_semaphores = Semaphore::handles(signal_semaphores);
  submit_info.signalSemaphoreCount = vk_signal_semaphores.size();
  submit_info.pSignalSemaphores = vk_signal_semaphores.data();

  auto vk_fence = (fence) ? fence->handle() : VK_NULL_HANDLE;
  if (vkQueueSubmit(queue_, 1, &submit_info, vk_fence) != VK_SUCCESS) {
    throw std::runtime_error("Failed to submit command buffer!");
  }
}

auto Queue::submitAndWaitIdle(const CommandBufferPtr& command_buffer,
                              const SemaphorePtrVec& wait_semaphores,
                              const PipelineStageFlagsVec& wait_stages,
                              const SemaphorePtrVec& signal_semaphores,
                              const FenceSharedPtr& fence) const -> void {
  submit(command_buffer, wait_semaphores, wait_stages, signal_semaphores,
         fence);
  vkQueueWaitIdle(queue_);
}

auto Queue::present(const SwapchainPtr& swapchain, uint32_t image_index,
                    const SemaphorePtrVec& wait_semaphores) const -> VkResult {
  VkPresentInfoKHR present_info{};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  auto vk_wait_semaphores = Semaphore::handles(wait_semaphores);
  present_info.waitSemaphoreCount = vk_wait_semaphores.size();
  present_info.pWaitSemaphores = vk_wait_semaphores.data();
  VkSwapchainKHR swapchains[] = {swapchain->handle()};
  present_info.swapchainCount = 1;
  present_info.pSwapchains = swapchains;
  present_info.pImageIndices = &image_index;
  present_info.pResults = nullptr;  // Optional
  return vkQueuePresentKHR(queue_, &present_info);
}
}  // namespace tupi
