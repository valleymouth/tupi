#pragma once

#include <memory>
#include <tuple>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/resource.h"
#include "tupi/queue_family.h"
#include "tupi/swapchain_support_detail.h"

namespace tupi {
class Swapchain : public internal::SharedResource<Swapchain> {
 public:
  Swapchain(Token, LogicalDevicePtr logical_device,
            SwapchainSupportDetail swapchain_support_detail,
            const QueueFamily& graphics_queue_family,
            const QueueFamily& present_queue_family, bool depth = true);
  ~Swapchain();

  auto handle() const -> VkSwapchainKHR;
  auto logicalDevice() const -> LogicalDevicePtr;
  auto extent() const -> VkExtent2D;
  auto format() const -> VkFormat;
  auto images() const -> Image2DPtrVec;
  auto sharingMode() const -> VkSharingMode;
  auto hasDepth() const -> bool;
  auto depthImageView() const -> ImageViewPtr;
  auto graphicsQueueFamilyIndex() const -> uint32_t;
  auto presentQueueFamilyIndex() const -> uint32_t;
  auto acquireNextImage(const SemaphorePtr& semaphore) const
      -> std::tuple<VkResult, uint32_t>;
  auto recreate() -> void;
  auto createFramebuffers(RenderPassPtr render_pass) -> void;

 protected:
  Swapchain(const Swapchain&) = delete;
  Swapchain(Swapchain&&) = delete;
  Swapchain& operator=(const Swapchain&) = delete;
  Swapchain& operator=(Swapchain&&) = delete;

 private:
  auto createSwapchain(bool depth) -> void;

  LogicalDevicePtr logical_device_{};
  SwapchainSupportDetail swapchain_support_detail_{};
  QueueFamily graphics_queue_family_{};
  QueueFamily present_queue_family_{};
  VkSwapchainKHR swapchain_{VK_NULL_HANDLE};
  VkExtent2D extent_{};
  VkFormat format_{};
  Image2DPtrVec images_{};
  Image2DPtr depth_image_{};
  ImageViewPtr depth_image_view_{};
  VkSharingMode sharing_mode_{VK_SHARING_MODE_EXCLUSIVE};
};

inline auto Swapchain::handle() const -> VkSwapchainKHR { return swapchain_; }

inline auto Swapchain::logicalDevice() const -> LogicalDevicePtr {
  return logical_device_;
}

inline auto Swapchain::extent() const -> VkExtent2D { return extent_; }

inline auto Swapchain::format() const -> VkFormat { return format_; }

inline auto Swapchain::images() const -> Image2DPtrVec { return images_; }

inline auto Swapchain::sharingMode() const -> VkSharingMode {
  return sharing_mode_;
}

inline auto Swapchain::hasDepth() const -> bool {
  return static_cast<bool>(depth_image_);
}

inline auto Swapchain::depthImageView() const -> ImageViewPtr {
  return depth_image_view_;
}

inline auto Swapchain::graphicsQueueFamilyIndex() const -> uint32_t {
  return graphics_queue_family_.index();
}

inline auto Swapchain::presentQueueFamilyIndex() const -> uint32_t {
  return present_queue_family_.index();
}
}  // namespace tupi
