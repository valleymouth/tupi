#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class Framebuffer {
 public:
  Framebuffer(LogicalDeviceSharedPtr logical_device, RenderPassPtr render_pass,
              ImageViewPtrVec image_views, VkExtent2D extent);
  ~Framebuffer();
  Framebuffer(const Framebuffer&) = delete;
  Framebuffer& operator=(const Framebuffer&) = delete;
  Framebuffer(Framebuffer&&) = default;
  Framebuffer& operator=(Framebuffer&&) = default;

  auto handle() const -> VkFramebuffer { return framebuffer_; }
  auto renderPass() const -> RenderPassPtr { return render_pass_; }
  auto extent() const -> VkExtent2D { return extent_; }

  static auto enumerate(const SwapchainPtr& swapchain,
                        const RenderPassPtr& render_pass) -> FramebufferPtrVec;

 private:
  LogicalDeviceSharedPtr logical_device_{};
  RenderPassPtr render_pass_{};
  ImageViewPtrVec image_views_{};
  VkExtent2D extent_{};
  Handle<VkFramebuffer> framebuffer_{};
};
}  // namespace tupi
