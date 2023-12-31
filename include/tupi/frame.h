#pragma once

#include "tupi/command_buffer.h"
#include "tupi/fence.h"
#include "tupi/fwd.h"
#include "tupi/internal/resource.h"

namespace tupi {
class Frame {
 public:
  Frame(const LogicalDevicePtr& logical_device,
        const CommandPoolPtr& command_pool);
  auto draw(SwapchainPtr& swapchain, FramebufferPtrVec& framebuffers,
            const PipelinePtr& pipeline, const Queue& graphics_queue,
            const Queue& present_queue, DescriptorSetPtrVec descriptor_sets,
            BufferPtrVec vertex_buffers, OffsetVec offsets,
            uint32_t vertex_count, BufferPtr index_buffer = {},
            Offset index_offset = 0, uint32_t index_count = 0) -> void;

 private:
  CommandBufferPtr command_buffer_{};
  SemaphorePtr image_available_semaphore_{};
  SemaphorePtr render_finished_semaphore_{};
  FencePtr fence_{};
};
}  // namespace tupi
