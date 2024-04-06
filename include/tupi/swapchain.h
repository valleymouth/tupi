#pragma once

#include <memory>
#include <tuple>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/queue_family.h"
#include "tupi/swapchain_support_detail.h"

namespace tupi {
class Swapchain {
 public:
  Swapchain(LogicalDeviceSharedPtr logical_device,
            SwapchainSupportDetail swapchain_support_detail,
            const QueueFamily& graphics_queue_family,
            const QueueFamily& present_queue_family, bool depth = true);
  ~Swapchain();
  Swapchain(const Swapchain&) = delete;
  Swapchain& operator=(const Swapchain&) = delete;
  Swapchain(Swapchain&&) = default;
  Swapchain& operator=(Swapchain&&) = default;

  auto handle() const -> VkSwapchainKHR { return swapchain_; }
  auto logicalDevice() const -> LogicalDeviceSharedPtr {
    return logical_device_;
  }
  auto extent() const -> VkExtent2D { return extent_; }
  auto width() const -> uint32_t { return extent_.width; }
  auto height() const -> uint32_t { return extent_.height; }
  auto format() const -> VkFormat { return format_; }
  auto sharingMode() const -> VkSharingMode { return sharing_mode_; }
  auto hasDepth() const -> bool { return static_cast<bool>(depth_image_); }
  auto depthImageView() const -> ImageViewPtr { return depth_image_view_; }
  auto graphicsQueueFamilyIndex() const -> uint32_t {
    return graphics_queue_family_.index();
  }
  auto presentQueueFamilyIndex() const -> uint32_t {
    return present_queue_family_.index();
  }
  auto acquireNextImage(const SemaphorePtr& semaphore) const
      -> std::tuple<VkResult, uint32_t>;
  auto recreate() -> void;
  auto createFramebuffers(RenderPassPtr render_pass) -> void;

 private:
  auto createSwapchain(bool depth) -> void;

  LogicalDeviceSharedPtr logical_device_{};
  SwapchainSupportDetail swapchain_support_detail_{};
  QueueFamily graphics_queue_family_{};
  QueueFamily present_queue_family_{};
  VkSwapchainKHR swapchain_{VK_NULL_HANDLE};
  VkExtent2D extent_{};
  VkFormat format_{};
  Image2DPtr depth_image_{};
  ImageViewPtr depth_image_view_{};
  VkSharingMode sharing_mode_{VK_SHARING_MODE_EXCLUSIVE};
};
}  // namespace tupi
