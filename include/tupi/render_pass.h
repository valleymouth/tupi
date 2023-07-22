#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"

namespace tupi {
class RenderPass : public internal::Creatable<RenderPass, std::shared_ptr> {
  friend class internal::Creatable<RenderPass, std::shared_ptr>;

 public:
  ~RenderPass();

  auto handle() const -> VkRenderPass;

 protected:
  RenderPass(LogicalDevicePtr logical_device,
             AttachmentDescriptionVec attachments,
             SubpassDescriptionVec subpasses);
  RenderPass(const RenderPass&) = delete;
  RenderPass(RenderPass&&) = delete;
  RenderPass& operator=(const RenderPass&) = delete;
  RenderPass& operator=(RenderPass&&) = delete;

 private:
  LogicalDevicePtr logical_device_;
  VkRenderPass render_pass_;
  AttachmentDescriptionVec attachments_;
  SubpassDescriptionVec subpasses_;
};

inline auto RenderPass::handle() const -> VkRenderPass { return render_pass_; }

}  // namespace tupi
