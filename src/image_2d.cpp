#include "tupi/image_2d.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>

#include "tupi/buffer.h"
#include "tupi/command_buffer.h"
#include "tupi/image_view.h"
#include "tupi/logical_device.h"
#include "tupi/physical_device.h"
#include "tupi/swapchain.h"

namespace tupi {
Image2D::Image2D(LogicalDeviceSharedPtr logical_device, const ImageInfo2D& info)
    : logical_device_(std::move(logical_device)), info_(info) {
  VkImageCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  create_info.imageType = VK_IMAGE_TYPE_2D;
  create_info.extent.width = info.extent.width;
  create_info.extent.height = info.extent.height;
  create_info.extent.depth = 1;
  create_info.mipLevels = info.mip_levels;
  create_info.arrayLayers = info.array_layers;
  create_info.format = info.format;
  create_info.tiling = info.tiling;
  create_info.initialLayout = info.initial_layout;
  create_info.usage = info.usage;
  create_info.samples = info.samples;
  create_info.flags = info.flags;
  create_info.sharingMode = info.sharing_mode;

  if (info.sharing_mode == VK_SHARING_MODE_CONCURRENT) {
    create_info.queueFamilyIndexCount = info.queue_family_indices.size();
    create_info.pQueueFamilyIndices = info.queue_family_indices.data();
  }

  if (vkCreateImage(logical_device_->handle(), &create_info, nullptr,
                    &image_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create image!");
  }
  info_ = info;

  VkMemoryRequirements requirements;
  vkGetImageMemoryRequirements(logical_device_->handle(), image_,
                               &requirements);

  VkMemoryAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = requirements.size;
  alloc_info.memoryTypeIndex = logical_device_->findMemoryType(
      requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  if (vkAllocateMemory(logical_device_->handle(), &alloc_info, nullptr,
                       &memory_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate image memory!");
  }

  vkBindImageMemory(logical_device_->handle(), image_, memory_, 0);
}

Image2D::~Image2D() {
  vkDestroyImage(logical_device_->handle(), handle(), nullptr);
  vkFreeMemory(logical_device_->handle(), memory_, nullptr);
}

auto Image2D::resize(const VkExtent2D& extent) -> void {
  // std::erase_if(image_views, [](auto&& x) { return x.expired(); });
  // for (const auto& image_view : image_views) {
  //   auto shared = image_view.lock();
  //   // shared->resize(extent);
  // }
}

auto Image2D::createFromFile(const CommandPoolSharedPtr& command_pool,
                             const Queue& queue,
                             const std::filesystem::path& path, VkFormat format)
    -> Image2DSharedPtr {
  int width, height, channels;
  // Force 4 channels for now since VK_FORMAT_R8G8B8_SRGB is not always
  // supported.
  int required_channels = 4;
  stbi_uc* pixels = stbi_load(path.string().c_str(), &width, &height, &channels,
                              required_channels);

  if (!pixels) {
    throw std::runtime_error("Failed to load texture image!");
  }

  VkDeviceSize size = width * height * required_channels;
  const auto& logical_device = command_pool->logicalDevice();
  auto buffer = Buffer::createShared<stbi_uc>(
      logical_device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  buffer->copy(pixels);
  stbi_image_free(pixels);

  auto result = std::make_shared<Image2D>(
      logical_device,
      ImageInfo2D(format,
                  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                  VkExtent2D{static_cast<uint32_t>(width),
                             static_cast<uint32_t>(height)}));

  {
    auto command_buffer = std::make_unique<CommandBuffer>(command_pool);
    command_buffer->transitionImageLayout(result, format,
                                          VK_IMAGE_LAYOUT_UNDEFINED,
                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    command_buffer->record(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    queue.submitAndWaitIdle(command_buffer);
  }
  {
    auto command_buffer = std::make_unique<CommandBuffer>(command_pool);
    command_buffer->copy(buffer, result);
    command_buffer->record(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    queue.submitAndWaitIdle(command_buffer);
  }
  {
    auto command_buffer = std::make_unique<CommandBuffer>(command_pool);
    command_buffer->transitionImageLayout(
        result, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    command_buffer->record(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    queue.submitAndWaitIdle(command_buffer);
  }

  return result;
}
}  // namespace tupi
