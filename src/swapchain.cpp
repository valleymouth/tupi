#include "tupi/swapchain.h"

#include <iostream>
#include <limits>

#include "tupi/framebuffer.h"
#include "tupi/image_2d.h"
#include "tupi/image_view_2d.h"
#include "tupi/logical_device.h"
#include "tupi/physical_device.h"
#include "tupi/semaphore.h"
#include "tupi/surface.h"
#include "tupi/swapchain_image.h"
#include "tupi/window.h"

namespace tupi {
Swapchain::Swapchain(LogicalDeviceSharedPtr logical_device,
                     SwapchainSupportDetail swapchain_support_detail,
                     const QueueFamily& graphics_queue_family,
                     const QueueFamily& present_queue_family, bool depth)
    : logical_device_(std::move(logical_device)),
      swapchain_support_detail_(std::move(swapchain_support_detail)),
      graphics_queue_family_(graphics_queue_family),
      present_queue_family_(present_queue_family) {
  createSwapchain(depth);
}

Swapchain::~Swapchain() {
  vkDestroySwapchainKHR(logical_device_->handle(), swapchain_, nullptr);
}

auto Swapchain::acquireNextImage(const SemaphoreSharedPtr& semaphore) const
    -> std::tuple<VkResult, uint32_t> {
  uint32_t image_index = std::numeric_limits<uint32_t>::max();
  auto result =
      vkAcquireNextImageKHR(logical_device_->handle(), swapchain_, UINT64_MAX,
                            semaphore->handle(), VK_NULL_HANDLE, &image_index);
  return {result, image_index};
}

auto Swapchain::recreate() -> void {
  vkDestroySwapchainKHR(logical_device_->handle(), swapchain_, nullptr);
  swapchain_support_detail_.updateSurfaceCapabilities();
  createSwapchain(static_cast<bool>(depth_image_));
}

auto Swapchain::createSwapchain(bool depth) -> void {
  const auto& physical_device = swapchain_support_detail_.physicalDevice();
  const auto& surface = swapchain_support_detail_.surface();
  const auto capabilities = swapchain_support_detail_.surfaceCapabilities();
  uint32_t image_count = capabilities.minImageCount + 1;
  const auto max_image_count = capabilities.maxImageCount;
  if (max_image_count > 0 && image_count > max_image_count) {
    image_count = max_image_count;
  }
  auto surface_format = swapchain_support_detail_.surfaceFormat();
  if (!surface_format.has_value()) {
    throw std::runtime_error(
        "Physical device doesn't have appropriate surface format!");
  }
  auto present_mode = swapchain_support_detail_.presentMode();
  if (!present_mode.has_value()) {
    throw std::runtime_error(
        "Physical device doesn't have appropriate present mode!");
  }
  auto frame_buffer_extent =
      swapchain_support_detail_.surface()->window()->extent();
  auto extent = swapchain_support_detail_.swapchainExtent(
      frame_buffer_extent.width, frame_buffer_extent.height);

  VkSwapchainCreateInfoKHR create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = surface->handle();
  create_info.minImageCount = image_count;
  create_info.imageFormat = surface_format.value().format;
  create_info.imageColorSpace = surface_format.value().colorSpace;
  create_info.imageExtent = extent;
  create_info.imageArrayLayers = 1;  // greater than one for stereoscopic 3D.
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  uint32_t queue_family_indices[] = {graphics_queue_family_.index(),
                                     present_queue_family_.index()};
  if (graphics_queue_family_.index() != present_queue_family_.index()) {
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices = queue_family_indices;
  } else {
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    create_info.queueFamilyIndexCount = 0;      // Optional
    create_info.pQueueFamilyIndices = nullptr;  // Optional
  }
  create_info.preTransform = capabilities.currentTransform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = present_mode.value();
  create_info.clipped = VK_TRUE;
  create_info.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(logical_device_->handle(), &create_info, nullptr,
                           &swapchain_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create swap chain!");
  }

  extent_ = extent;
  format_ = surface_format.value().format;

  // Depth buffer
  if (depth) {
    auto depth_format = physical_device->findDepthFormat();
    auto depth_image_info = tupi::ImageInfo2D(
        depth_format, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, extent);
    depth_image_ =
        std::make_shared<tupi::Image2D>(logical_device_, depth_image_info);
    depth_image_view_ = std::make_shared<tupi::ImageView2D>(
        logical_device_, depth_image_, depth_format, VK_IMAGE_ASPECT_DEPTH_BIT);
  }
}
}  // namespace tupi
