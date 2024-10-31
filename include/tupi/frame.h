#pragma once

#include <chrono>

#include "tupi/command_buffer.h"
#include "tupi/fence.h"
#include "tupi/fwd.h"

namespace tupi {

class Frame {
 public:
  Frame(const LogicalDeviceSharedPtr& logical_device,
        const CommandPoolSharedPtr& command_pool);
  auto draw(SwapchainSharedPtr& swapchain,
            FramebufferSharedPtrVec& framebuffers,
            const PipelineSharedPtr& pipeline, const Queue& graphics_queue,
            const Queue& present_queue,
            DescriptorSetSharedPtrVec descriptor_sets,
            BufferSharedPtrVec vertex_buffers, OffsetVec offsets,
            uint32_t vertex_count, BufferSharedPtr index_buffer = {},
            Offset index_offset = 0, uint32_t index_count = 0) -> void;

  auto addTickObserver(ITickObserverSharedPtr tick_observer) -> void;

 private:
  CommandBufferPtr command_buffer_{};
  SemaphoreSharedPtr image_available_semaphore_{};
  SemaphoreSharedPtr render_finished_semaphore_{};
  FenceSharedPtr fence_{};
  ITickObserverSharedPtrVec tick_observers_{};

  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;
  TimePoint last_frame_time_point_;
};
}  // namespace tupi
