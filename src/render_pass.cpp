#include "tupi/render_pass.h"

#include "tupi/logical_device.h"
#include "tupi/subpass_description.h"

namespace tupi {
RenderPass::~RenderPass() {
  vkDestroyRenderPass(logical_device_->handle(), render_pass_, nullptr);
}

RenderPass::RenderPass(LogicalDevicePtr logical_device,
                       AttachmentDescriptionVec attachments,
                       SubpassDescriptionVec subpasses)
    : logical_device_(std::move(logical_device)),
      attachments_(std::move(attachments)),
      subpasses_(std::move(subpasses)) {
  VkRenderPassCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  create_info.attachmentCount = attachments_.size();
  std::vector<VkAttachmentDescription> vk_attachments;
  vk_attachments.reserve(attachments_.size());
  for (const auto& attachment : attachments_) {
    vk_attachments.emplace_back(attachment);
  }
  create_info.pAttachments = vk_attachments.data();
  create_info.subpassCount = subpasses_.size();
  std::vector<VkSubpassDescription> vk_subpasses;
  vk_subpasses.reserve(subpasses_.size());
  for (const auto& subpass : subpasses_) {
    vk_subpasses.emplace_back(subpass.handle());
  }
  create_info.pSubpasses = vk_subpasses.data();

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  create_info.dependencyCount = 1;
  create_info.pDependencies = &dependency;

  if (vkCreateRenderPass(logical_device_->handle(), &create_info, nullptr,
                         &render_pass_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create render pass!");
  }
}
}  // namespace tupi
