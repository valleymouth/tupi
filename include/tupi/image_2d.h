#pragma once

#include <filesystem>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/image.h"

namespace tupi {
/// @brief Image has too many parameters.
struct ImageInfo2D {
  VkFormat format;
  VkImageUsageFlags usage;
  VkExtent2D extent{0, 0};
  uint32_t mip_levels{1};
  uint32_t array_layers{1};
  VkImageCreateFlags flags{0};
  VkImageTiling tiling{VK_IMAGE_TILING_OPTIMAL};
  VkSharingMode sharing_mode{VK_SHARING_MODE_EXCLUSIVE};
  VkSampleCountFlagBits samples{VK_SAMPLE_COUNT_1_BIT};
  VkImageLayout initial_layout{VK_IMAGE_LAYOUT_UNDEFINED};
  std::vector<uint32_t> queue_family_indices;

  ImageInfo2D(VkFormat format, VkImageUsageFlags usage, VkExtent2D extent)
      : format(format), usage(usage), extent(extent) {}
};

class Image2D : public Image {
 public:
  ~Image2D();

  auto width() const -> uint32_t;
  auto height() const -> uint32_t;

  static auto create(LogicalDevicePtr logical_device, const ImageInfo2D& info)
      -> Image2DPtr;
  static auto create(const CommandPoolPtr& command_pool, const Queue& queue,
                     const std::filesystem::path& path) -> Image2DPtr;

 protected:
  Image2D(LogicalDevicePtr logical_device, VkImage image,
          const ImageInfo2D& info)
      : Image(std::move(logical_device), image), info_(info) {}
  Image2D(const Image2D&) = delete;
  Image2D(Image2D&&);
  auto operator=(const Image2D&) -> Image& = delete;
  auto operator=(Image2D&& other) -> Image&;

 private:
  ImageInfo2D info_;
  VkDeviceMemory memory_{VK_NULL_HANDLE};
};

inline auto Image2D::width() const -> uint32_t { return info_.extent.width; }

inline auto Image2D::height() const -> uint32_t { return info_.extent.height; }
}  // namespace tupi
