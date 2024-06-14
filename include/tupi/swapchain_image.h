#pragma once

#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/image.h"
#include "tupi/swapchain.h"

namespace tupi {
class SwapchainImage : public IImage {
  struct Private {};

 public:
  SwapchainImage(Private, SwapchainSharedPtr swapchain, VkImage image);

  auto swapchain() const -> SwapchainSharedPtr { return swapchain_; }
  auto width() const -> uint32_t { return swapchain_->width(); }
  auto height() const -> uint32_t { return swapchain_->height(); }

  auto handle() const -> VkImage override { return image_; };
  auto logicalDevice() const -> LogicalDeviceSharedPtr override {
    return swapchain_->logicalDevice();
  }
  auto is2D() const -> bool override { return true; }
  auto format() const -> VkFormat override { return VK_FORMAT_UNDEFINED; }

  static auto enumerate(const SwapchainSharedPtr& swapchain)
      -> IImageSharedPtrVec;

 private:
  SwapchainSharedPtr swapchain_{};
  VkImage image_{VK_NULL_HANDLE};
};
}  // namespace tupi