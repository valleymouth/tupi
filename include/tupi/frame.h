#pragma once

#include "tupi/command_buffer.h"
#include "tupi/fence.h"
#include "tupi/fwd.h"

namespace tupi {
class Frame {
 public:
  Frame(const LogicalDevicePtr& logical_device,
        const CommandPoolPtr& command_pool);

  auto draw(SwapchainPtr& swapchain, FramebufferPtrVec& framebuffers,
            const PipelinePtr& pipeline, const Queue& graphics_queue,
            const Queue& present_queue) -> void;

 private:
  CommandBuffer command_buffer_{};
  SemaphorePtr image_available_semaphore_{};
  SemaphorePtr render_finished_semaphore_{};
  FencePtr fence_{};
};
}  // namespace tupi
