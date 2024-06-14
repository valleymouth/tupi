#include "tupi/image_view_2d.h"

#include "tupi/image.h"
#include "tupi/logical_device.h"

namespace tupi {
ImageView2D::ImageView2D(LogicalDeviceSharedPtr logical_device,
                         IImageSharedPtr image, const VkFormat& format,
                         VkImageAspectFlags aspect_flags)
    : logical_device_(std::move(logical_device)), image_(std::move(image)) {
  if (!image_->is2D()) {
    throw std::runtime_error(
        "2D image view needs a 2D image, failed to create image view!");
  }

  VkImageViewCreateInfo create_info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
  create_info.image = image_->handle();
  create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  create_info.format = format;
  create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  create_info.subresourceRange.aspectMask = aspect_flags;
  create_info.subresourceRange.baseMipLevel = 0;
  create_info.subresourceRange.levelCount = 1;
  create_info.subresourceRange.baseArrayLayer = 0;
  create_info.subresourceRange.layerCount = 1;
  if (vkCreateImageView(logical_device_->handle(), &create_info, nullptr,
                        &image_view_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create image view!");
  }
}

ImageView2D::~ImageView2D() {
  vkDestroyImageView(logical_device_->handle(), image_view_, nullptr);
}
}  // namespace tupi
