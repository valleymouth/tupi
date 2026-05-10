#ifndef TUPI_RENDER_PASS_H
#define TUPI_RENDER_PASS_H

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/subpass_description.h"

namespace tupi {
class RenderPass {
 public:
  RenderPass(LogicalDeviceSharedPtr logical_device,
             AttachmentDescriptionVec attachments,
             SubpassDescriptionVec subpasses);
  ~RenderPass();
  RenderPass(const RenderPass&) = delete;
  RenderPass& operator=(const RenderPass&) = delete;
  RenderPass(RenderPass&&) = default;
  RenderPass& operator=(RenderPass&&) = default;

  auto logicalDevice() const -> LogicalDeviceSharedPtr {
    return logical_device_;
  }

  auto handle() const -> VkRenderPass { return render_pass_; }

 private:
  LogicalDeviceSharedPtr logical_device_{};
  Handle<VkRenderPass> render_pass_{};
  AttachmentDescriptionVec attachments_{};
  SubpassDescriptionVec subpasses_{};
};
}  // namespace tupi
#endif  // TUPI_RENDER_PASS_H