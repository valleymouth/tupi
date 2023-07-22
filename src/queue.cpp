#include "tupi/queue.h"

#include "tupi/command_buffer.h"
#include "tupi/fence.h"
#include "tupi/logical_device.h"
#include "tupi/queue_family.h"
#include "tupi/semaphore.h"
#include "tupi/swapchain.h"

namespace tupi {
Queue::Queue(LogicalDevicePtr device, const QueueFamily& queue_family,
             uint32_t queue_index)
    : logical_device_(std::move(device)) {
  vkGetDeviceQueue(logical_device_->handle(), queue_family.index(), queue_index,
                   &queue_);
}

auto Queue::submit(const SemaphorePtrVec& wait_semaphores,
                   const PipelineStageFlagsVec& wait_stages,
                   const CommandBuffer& command_buffer,
                   const SemaphorePtrVec& signal_semaphores,
                   const FencePtr& fence) const -> void {
  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  std::vector<VkSemaphore> vk_wait_semaphores;
  vk_wait_semaphores.reserve(wait_semaphores.size());
  for (const auto& semaphore : wait_semaphores) {
    vk_wait_semaphores.emplace_back(semaphore->handle());
  }
  submit_info.waitSemaphoreCount = vk_wait_semaphores.size();
  submit_info.pWaitSemaphores = vk_wait_semaphores.data();
  submit_info.pWaitDstStageMask = wait_stages.data();
  auto vk_command_buffer = command_buffer.handle();
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &vk_command_buffer;
  std::vector<VkSemaphore> vk_signal_semaphores;
  vk_signal_semaphores.reserve(signal_semaphores.size());
  for (const auto& semaphore : signal_semaphores) {
    vk_signal_semaphores.emplace_back(semaphore->handle());
  }
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = vk_signal_semaphores.data();

  if (vkQueueSubmit(queue_, 1, &submit_info, fence->handle()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to submit command buffer!");
  }
}

auto Queue::present(const SwapchainPtr& swapchain, uint32_t image_index,
                    const SemaphorePtrVec& wait_semaphores) const -> VkResult {
  VkPresentInfoKHR present_info{};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  std::vector<VkSemaphore> vk_wait_semaphores;
  vk_wait_semaphores.reserve(wait_semaphores.size());
  for (const auto& semaphore : wait_semaphores) {
    vk_wait_semaphores.emplace_back(semaphore->handle());
  }
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
