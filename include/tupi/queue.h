#pragma once

#include "tupi/fwd.h"

namespace tupi {
class Queue {
 public:
  Queue(LogicalDevicePtr device, const QueueFamily& queue_family,
        uint32_t queue_index);

  auto submit(const CommandBuffer& command_buffer,
              const SemaphorePtrVec& wait_semaphores = {},
              const PipelineStageFlagsVec& wait_states = {},
              const SemaphorePtrVec& signal_semaphores = {},
              const FencePtr& fence = {}, bool wait_idle = false) const -> void;
  auto present(const SwapchainPtr& swapchain, uint32_t image_index,
               const SemaphorePtrVec& wait_semaphores) const -> VkResult;

 private:
  LogicalDevicePtr logical_device_{};
  VkQueue queue_{VK_NULL_HANDLE};
};
}  // namespace tupi
