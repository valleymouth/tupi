#pragma once

#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/image.h"
#include "tupi/swapchain.h"


namespace tupi {
class SwapchainImage : public IImage {
  struct Private {};

 public:
  SwapchainImage(Private, SwapchainPtr swapchain, VkImage image);

  auto swapchain() const -> SwapchainPtr { return swapchain_; }
  auto width() const -> uint32_t { return swapchain_->width(); }
  auto height() const -> uint32_t { return swapchain_->height(); }

  auto handle() const -> VkImage override { return image_; };
  auto logicalDevice() const -> LogicalDeviceSharedPtr override {
    return swapchain_->logicalDevice();
  }

  static auto enumerate(const SwapchainPtr& swapchain)
      -> SwapchainImageSharedPtrVec;

 private:
  SwapchainPtr swapchain_{};
  VkImage image_{VK_NULL_HANDLE};
};
}  // namespace tupi