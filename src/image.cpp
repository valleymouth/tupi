#include "tupi/image.h"

#include <vector>

#include "tupi/logical_device.h"
#include "tupi/swapchain.h"

namespace tupi {
auto Image::enumerate(const Swapchain& swapchain) -> ImagePtrVec {
  uint32_t count;
  const auto& logical_device = swapchain.logicalDevice();
  vkGetSwapchainImagesKHR(logical_device->handle(), swapchain.handle(), &count,
                          nullptr);
  std::vector<VkImage> images(count);
  vkGetSwapchainImagesKHR(logical_device->handle(), swapchain.handle(), &count,
                          images.data());
  ImagePtrVec result;
  result.reserve(count);
  for (const auto& vk_image : images) {
    auto& image = result.emplace_back(
        std::shared_ptr<Image>(new Image(logical_device, vk_image)));
  }
  return result;
}

Image::Image(Image&& other)
    : logical_device_(std::move(other.logical_device_)), image_(other.image_) {
  other.image_ = VK_NULL_HANDLE;
}

auto Image::operator=(Image&& other) -> Image& {
  logical_device_ = std::move(other.logical_device_);
  image_ = other.image_;
  other.image_ = VK_NULL_HANDLE;
  return *this;
}
}  // namespace tupi
