#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"

namespace tupi {
class ImageView : public internal::Creatable<ImageView, std::shared_ptr> {
  friend class internal::Creatable<ImageView, std::shared_ptr>;

 public:
  ~ImageView();

  auto handle() const -> VkImageView;

 protected:
  ImageView(LogicalDevicePtr logical_device, ImagePtr image,
            const VkFormat& format);
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
