#pragma once

#include "tupi/fwd.h"

namespace tupi {
class Queue {
 public:
  Queue(LogicalDevicePtr device, const QueueFamily& queue_family,
        uint32_t queue_index);

  auto submit(const CommandBufferPtr& command_buffer,
              const SemaphorePtrVec& wait_semaphores = {},
              const PipelineStageFlagsVec& wait_stages = {},
              const SemaphorePtrVec& signal_semaphores = {},
              const FencePtr& fence = {}) const -> void;
  auto submitAndWaitIdle(const CommandBufferPtr& command_buffer,
                         const SemaphorePtrVec& wait_semaphores = {},
                         const PipelineStageFlagsVec& wait_stages = {},
                         const SemaphorePtrVec& signal_semaphores = {},
                         const FencePtr& fence = {}) const -> void;
  auto present(const SwapchainPtr& swapchain, uint32_t image_index,
               const SemaphorePtrVec& wait_semaphores) const -> VkResult;

 private:
  LogicalDevicePtr logical_device_{};
  VkQueue queue_{VK_NULL_HANDLE};
};
}  // namespace tupi
