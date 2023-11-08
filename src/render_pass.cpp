#include "tupi/render_pass.h"

#include "tupi/logical_device.h"
#include "tupi/subpass_description.h"

namespace tupi {
RenderPass::RenderPass(Token, LogicalDevicePtr logical_device,
                       AttachmentDescriptionVec attachments,
                       SubpassDescriptionVec subpasses)
    : logical_device_(std::move(logical_device)),
      attachments_(std::move(attachments)),
      subpasses_(std::move(subpasses)) {
  VkRenderPassCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  create_info.attachmentCount = static_cast<uint32_t>(attachments_.size());
  create_info.pAttachments = attachments_.data();
  create_info.subpassCount = static_cast<uint32_t>(subpasses_.size());
  auto vk_subpasses = SubpassDescription::handles(subpasses_);
  create_info.pSubpasses = vk_subpasses.data();

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcAccessMask = 0;
  // TODO: Not sure how to expose this. Maybe a dependency vector needs to be
  // provided.
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                             VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  create_info.dependencyCount = 1;
  create_info.pDependencies = &dependency;

  if (vkCreateRenderPass(logical_device_->handle(), &create_info, nullptr,
                         &render_pass_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create render pass!");
  }
}

RenderPass::~RenderPass() {
  vkDestroyRenderPass(logical_device_->handle(), render_pass_, nullptr);
}
}  // namespace tupi
