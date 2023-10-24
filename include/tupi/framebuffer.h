#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"

namespace tupi {
class Framebuffer : public internal::Creatable<Framebuffer, std::shared_ptr> {
  friend class internal::Creatable<Framebuffer, std::shared_ptr>;

 public:
  ~Framebuffer();

  auto handle() const -> VkFramebuffer;
  auto renderPass() const -> RenderPassPtr;
  auto extent() const -> VkExtent2D;

  static auto enumerate(const Swapchain& swapchain,
                        const RenderPassPtr& render_pass) -> FramebufferPtrVec;

 protected:
  Framebuffer(LogicalDevicePtr logical_device, RenderPassPtr render_pass,
              ImageViewPtrVec image_views, VkExtent2D extent);
  Framebuffer(const Framebuffer&) = delete;
  Framebuffer(Framebuffer&&) = delete;
  Framebuffer& operator=(const Framebuffer&) = delete;
  Framebuffer& operator=(Framebuffer&&) = delete;

 private:
  LogicalDevicePtr logical_device_{};
  RenderPassPtr render_pass_{};
  ImageViewPtrVec image_views_{};
  VkExtent2D extent_{};
  VkFramebuffer framebuffer_{VK_NULL_HANDLE};
};

inline auto Framebuffer::handle() const -> VkFramebuffer {
  return framebuffer_;
}

inline auto Framebuffer::renderPass() const -> RenderPassPtr {
  return render_pass_;
}

inline auto Framebuffer::extent() const -> VkExtent2D { return extent_; }
}  // namespace tupi
