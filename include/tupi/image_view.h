#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/resource.h"

namespace tupi {
class ImageView : public internal::SharedResource<ImageView> {
 public:
  ImageView(Token, LogicalDevicePtr logical_device, ImagePtr image,
            const VkFormat& format,
            VkImageAspectFlags aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT);
  ~ImageView();

  auto handle() const -> VkImageView;
  auto resize(const VkExtent2D& extent) -> void;

 protected:
  ImageView(const ImageView&) = delete;
  ImageView(ImageView&&) = delete;
  ImageView& operator=(const ImageView&) = delete;
  ImageView& operator=(ImageView&&) = delete;

 private:
  LogicalDevicePtr logical_device_;
  ImagePtr image_;
  VkImageView image_view_{VK_NULL_HANDLE};
};

inline auto ImageView::handle() const -> VkImageView { return image_view_; }
}  // namespace tupi
