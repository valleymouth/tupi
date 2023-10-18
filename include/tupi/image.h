#pragma once

#include <filesystem>
#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"

namespace tupi {
class Image {
 public:
  auto handle() const -> VkImage;
  auto logical_device() const -> LogicalDevicePtr;

  static auto enumerate(const Swapchain& swapchain) -> ImagePtrVec;

 protected:
  Image(LogicalDevicePtr logical_device, VkImage image)
      : logical_device_(std::move(logical_device)), image_(image) {}
  Image(const Image&) = default;
  Image(Image&& other);
  auto operator=(const Image&) -> Image& = default;
  auto operator=(Image&& other) -> Image&;

 private:
  LogicalDevicePtr logical_device_{};
  VkImage image_{VK_NULL_HANDLE};
};

inline auto Image::handle() const -> VkImage { return image_; }

inline auto Image::logical_device() const -> LogicalDevicePtr {
  return logical_device_;
}
}  // namespace tupi
