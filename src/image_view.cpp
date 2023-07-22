#include "tupi/image_view.h"

#include "tupi/image.h"
#include "tupi/logical_device.h"

namespace tupi {
ImageView::~ImageView() {
  vkDestroyImageView(logical_device_->handle(), image_view_, nullptr);
}

ImageView::ImageView(LogicalDevicePtr logical_device, ImagePtr image,
                     const VkFormat& format)
    : logical_device_(std::move(logical_device)), image_(std::move(image)) {
  VkImageViewCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  create_info.image = image_->handle();
  create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  create_info.format = format;
  create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  create_info.subresourceRange.baseMipLevel = 0;
  create_info.subresourceRange.levelCount = 1;
  create_info.subresourceRange.baseArrayLayer = 0;
  create_info.subresourceRange.layerCount = 1;
  if (vkCreateImageView(logical_device_->handle(), &create_info, nullptr,
                        &image_view_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create image views!");
  }
}
}  // namespace tupi
