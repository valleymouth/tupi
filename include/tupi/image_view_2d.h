#ifndef TUPI_IMAGE_VIEW_2D_H
#define TUPI_IMAGE_VIEW_2D_H

#include "tupi/fwd.h"
#include "tupi/image_view.h"

namespace tupi {
class ImageView2D : public IImageView {
 public:
  ImageView2D(LogicalDeviceSharedPtr logical_device, IImageSharedPtr image,
              const VkFormat& format,
              VkImageAspectFlags aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT);
  ~ImageView2D();
  ImageView2D(const ImageView2D&) = delete;
  ImageView2D& operator=(const ImageView2D&) = delete;
  ImageView2D(ImageView2D&& other) = default;
  ImageView2D& operator=(ImageView2D&& other) = default;

  auto handle() const -> VkImageView override { return image_view_; }
  auto logicalDevice() const -> LogicalDeviceSharedPtr override {
    return logical_device_;
  }
  auto is2D() const -> bool override { return true; }

  auto resize(const VkExtent2D& extent) -> void;

 private:
  LogicalDeviceSharedPtr logical_device_;
  IImageSharedPtr image_;
  Handle<VkImageView> image_view_{};
};
}  // namespace tupi
#endif  // TUPI_IMAGE_VIEW_2D_H