#pragma once

#include <filesystem>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/image.h"

namespace tupi {
/// @brief Image has too many parameters.
struct ImageInfo2D {
  VkFormat format{VK_FORMAT_UNDEFINED};
  VkImageUsageFlags usage{0};
  VkExtent2D extent{0, 0};
  uint32_t mip_levels{1};
  uint32_t array_layers{1};
  VkImageCreateFlags flags{0};
  VkImageTiling tiling{VK_IMAGE_TILING_OPTIMAL};
  VkSharingMode sharing_mode{VK_SHARING_MODE_EXCLUSIVE};
  VkSampleCountFlagBits samples{VK_SAMPLE_COUNT_1_BIT};
  VkImageLayout initial_layout{VK_IMAGE_LAYOUT_UNDEFINED};
  std::vector<uint32_t> queue_family_indices;

  ImageInfo2D() = default;
  ImageInfo2D(VkFormat format, VkImageUsageFlags usage, VkExtent2D extent)
      : format(format), usage(usage), extent(extent) {}
};

class Image2D : public IImage {
 public:
  Image2D(LogicalDeviceSharedPtr logical_device, const ImageInfo2D& info);
  ~Image2D();
  Image2D(const Image2D&) = delete;
  Image2D& operator=(const Image2D&) = delete;
  Image2D(Image2D&&) = default;
  Image2D& operator=(Image2D&&) = default;

  auto handle() const -> VkImage override { return image_; }
  auto logicalDevice() const -> LogicalDeviceSharedPtr override {
    return logical_device_;
  }
  auto is2D() const -> bool override { return true; }
  auto format() const -> VkFormat override { return info_.format; }

  auto width() const -> uint32_t { return info_.extent.width; }
  auto height() const -> uint32_t { return info_.extent.height; }
  auto resize(const VkExtent2D& extent) -> void;

  static auto createFromFile(const CommandPoolSharedPtr& command_pool,
                             const Queue& queue,
                             const std::filesystem::path& path, VkFormat format)
      -> Image2DSharedPtr;

 private:
  LogicalDeviceSharedPtr logical_device_{};
  ImageInfo2D info_{};
  Handle<VkImage> image_{};
  Handle<VkDeviceMemory> memory_{};
};
}  // namespace tupi
