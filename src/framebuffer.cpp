#include "tupi/framebuffer.h"

#include "tupi/image_2d.h"
#include "tupi/image_view.h"
#include "tupi/logical_device.h"
#include "tupi/render_pass.h"
#include "tupi/swapchain.h"

namespace tupi {
Framebuffer::Framebuffer(Token, LogicalDevicePtr logical_device,
                         RenderPassPtr render_pass, ImageViewPtrVec image_views,
                         VkExtent2D extent)
    : logical_device_(std::move(logical_device)),
      render_pass_(std::move(render_pass)),
      image_views_(std::move(image_views)),
      extent_(extent) {
  std::vector<VkImageView> attachments;
  attachments.reserve(image_views_.size());
  for (const auto& image_view : image_views_) {
    attachments.emplace_back(image_view->handle());
  }

  VkFramebufferCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  create_info.renderPass = render_pass_->handle();
  create_info.attachmentCount = attachments.size();
  create_info.pAttachments = attachments.data();
  create_info.width = extent_.width;
  create_info.height = extent_.height;
  create_info.layers = 1;

  if (vkCreateFramebuffer(logical_device_->handle(), &create_info, nullptr,
                          &framebuffer_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create framebuffer!");
  }
}

Framebuffer::~Framebuffer() {
  vkDestroyFramebuffer(logical_device_->handle(), framebuffer_, nullptr);
}

auto Framebuffer::enumerate(const Swapchain& swapchain,
                            const RenderPassPtr& render_pass)
    -> FramebufferPtrVec {
  auto logical_device = swapchain.logicalDevice();
  auto images = swapchain.images();
  tupi::FramebufferPtrVec framebuffers;
  framebuffers.reserve(images.size());
  for (auto& image : images) {
    auto image_view =
        ImageView::create(logical_device, image, swapchain.format());
    auto image_views = tupi::ImageViewPtrVec{image_view};
    if (swapchain.hasDepth()) {
      image_views.emplace_back(swapchain.depthImageView());
    }
    framebuffers.emplace_back(tupi::Framebuffer::create(
        logical_device, render_pass, image_views, swapchain.extent()));
  }
  return framebuffers;
}
}  // namespace tupi
