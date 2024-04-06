#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class ImageView {
 public:
  ImageView(LogicalDeviceSharedPtr logical_device, IImagePtr image,
            const VkFormat& format,
            VkImageAspectFlags aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT);
  ~ImageView();
  ImageView(const ImageView&) = delete;
  ImageView& operator=(const ImageView&) = delete;
  ImageView(ImageView&& other) = default;
  ImageView& operator=(ImageView&& other) = default;

  auto handle() const -> VkImageView { return image_view_; }
  auto resize(const VkExtent2D& extent) -> void;

 private:
  LogicalDeviceSharedPtr logical_device_;
  IImagePtr image_;
  Handle<VkImageView> image_view_{};
};
}  // namespace tupi
