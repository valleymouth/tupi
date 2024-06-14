#pragma once

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class Queue {
 public:
  Queue(LogicalDeviceSharedPtr device, const QueueFamily& queue_family,
        uint32_t queue_index);

  auto submit(const CommandBufferPtr& command_buffer,
              const SemaphoreSharedPtrVec& wait_semaphores = {},
              const PipelineStageFlagsVec& wait_stages = {},
              const SemaphoreSharedPtrVec& signal_semaphores = {},
              const FenceSharedPtr& fence = {}) const -> void;
  auto submitAndWaitIdle(const CommandBufferPtr& command_buffer,
                         const SemaphoreSharedPtrVec& wait_semaphores = {},
                         const PipelineStageFlagsVec& wait_stages = {},
                         const SemaphoreSharedPtrVec& signal_semaphores = {},
                         const FenceSharedPtr& fence = {}) const -> void;
  auto present(const SwapchainSharedPtr& swapchain, uint32_t image_index,
               const SemaphoreSharedPtrVec& wait_semaphores) const -> VkResult;

 private:
  LogicalDeviceSharedPtr logical_device_{};
  Handle<VkQueue> queue_{};
};
}  // namespace tupi
