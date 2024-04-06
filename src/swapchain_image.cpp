#include "tupi/swapchain_image.h"

#include "tupi/logical_device.h"
#include "tupi/swapchain.h"

namespace tupi {
SwapchainImage::SwapchainImage(SwapchainImage::Private, SwapchainPtr swapchain,
                               VkImage image)
    : swapchain_(std::move(swapchain)), image_(image) {}

auto SwapchainImage::enumerate(const SwapchainPtr& swapchain)
    -> SwapchainImageSharedPtrVec {
  uint32_t count;
  const auto& logical_device = swapchain->logicalDevice();
  vkGetSwapchainImagesKHR(logical_device->handle(), swapchain->handle(), &count,
                          nullptr);
  std::vector<VkImage> images(count);
  vkGetSwapchainImagesKHR(logical_device->handle(), swapchain->handle(), &count,
                          images.data());
  SwapchainImageSharedPtrVec result;
  result.reserve(count);
  for (const auto& vk_image : images) {
    auto& image = result.emplace_back(
        std::make_shared<SwapchainImage>(Private{}, swapchain, vk_image));
  }
  return result;
}
}  // namespace tupi