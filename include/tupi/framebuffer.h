#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class Framebuffer {
 public:
  Framebuffer(LogicalDeviceSharedPtr logical_device,
              RenderPassSharedPtr render_pass,
              ImageView2DSharedPtrVec image_views, VkExtent2D extent);
  ~Framebuffer();
  Framebuffer(const Framebuffer&) = delete;
  Framebuffer& operator=(const Framebuffer&) = delete;
  Framebuffer(Framebuffer&&) = default;
  Framebuffer& operator=(Framebuffer&&) = default;

  auto handle() const -> VkFramebuffer { return framebuffer_; }
  auto renderPass() const -> RenderPassSharedPtr { return render_pass_; }
  auto extent() const -> VkExtent2D { return extent_; }

  static auto enumerate(const SwapchainSharedPtr& swapchain,
                        const RenderPassSharedPtr& render_pass)
      -> FramebufferSharedPtrVec;

 private:
  LogicalDeviceSharedPtr logical_device_{};
  RenderPassSharedPtr render_pass_{};
  ImageView2DSharedPtrVec image_views_{};
  VkExtent2D extent_{};
  Handle<VkFramebuffer> framebuffer_{};
};
}  // namespace tupi
