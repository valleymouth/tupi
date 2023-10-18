#include "tupi/image_2d.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>

#include "tupi/buffer.h"
#include "tupi/command_buffer.h"
#include "tupi/logical_device.h"
#include "tupi/swapchain.h"

namespace tupi {
Image2D::~Image2D() {
  vkDestroyImage(logical_device()->handle(), handle(), nullptr);
  vkFreeMemory(logical_device()->handle(), memory_, nullptr);
}

auto Image2D::create(LogicalDevicePtr logical_device, const ImageInfo2D& info)
    -> Image2DPtr {
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

  VkImage vk_image;
  if (vkCreateImage(logical_device->handle(), &create_info, nullptr,
                    &vk_image) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create image!");
  }
  auto result =
      std::shared_ptr<Image2D>(new Image2D(logical_device, vk_image, info));

  VkMemoryRequirements requirements;
  vkGetImageMemoryRequirements(logical_device->handle(), vk_image,
                               &requirements);

  VkMemoryAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = requirements.size;
  alloc_info.memoryTypeIndex = logical_device->findMemoryType(
      requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  if (vkAllocateMemory(logical_device->handle(), &alloc_info, nullptr,
                       &result->memory_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate image memory!");
  }

  vkBindImageMemory(logical_device->handle(), vk_image, result->memory_, 0);

  return result;
}

auto Image2D::create(const CommandPoolPtr& command_pool, const Queue& queue,
                     const std::filesystem::path& path) -> Image2DPtr {
  int width, height, channels;
  stbi_uc* pixels =
      stbi_load(path.string().c_str(), &width, &height, &channels, 0);

  if (!pixels) {
    throw std::runtime_error("Failed to load texture image!");
  }

  VkDeviceSize size = width * height * channels;
  const auto& logical_device = command_pool->logicalDevice();
  auto buffer = Buffer::create<stbi_uc>(
      logical_device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  buffer->copy(pixels);
  stbi_image_free(pixels);

  auto result = Image2D::create(
      logical_device,
      ImageInfo2D(VK_FORMAT_R8G8B8A8_SRGB,
                  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                  VkExtent2D{static_cast<uint32_t>(width),
                             static_cast<uint32_t>(height)}));

  {
    auto command_buffer = CommandBuffer::create(command_pool);
    command_buffer->transitionImageLayout(result, VK_FORMAT_R8G8B8A8_SRGB,
                                          VK_IMAGE_LAYOUT_UNDEFINED,
                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    command_buffer->record(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    queue.submitAndWaitIdle(command_buffer);
  }
  {
    auto command_buffer = CommandBuffer::create(command_pool);
    command_buffer->copy(buffer, result);
    command_buffer->record(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    queue.submitAndWaitIdle(command_buffer);
  }
  {
    auto command_buffer = CommandBuffer::create(command_pool);
    command_buffer->transitionImageLayout(
        result, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    command_buffer->record(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    queue.submitAndWaitIdle(command_buffer);
  }

  return result;
}

Image2D::Image2D(Image2D&& other)
    : Image(std::move(other)), info_(other.info_), memory_(other.memory_) {
  other.memory_ = VK_NULL_HANDLE;
}

auto Image2D::operator=(Image2D&& other) -> Image& {
  Image::operator=(std::move(other));
  info_ = other.info_;
  memory_ = other.memory_;
  other.memory_ = VK_NULL_HANDLE;
  return *this;
}
}  // namespace tupi
