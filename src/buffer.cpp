#include "tupi/buffer.h"

#include "tupi/physical_device.h"

namespace tupi {
Buffer::~Buffer() {
  vkDestroyBuffer(logical_device_->handle(), buffer_, nullptr);
  vkFreeMemory(logical_device_->handle(), memory_, nullptr);
}

auto Buffer::memoryRequirements() const -> VkMemoryRequirements {
  VkMemoryRequirements requirements;
  vkGetBufferMemoryRequirements(logical_device_->handle(), buffer_,
                                &requirements);
  return requirements;
}

auto Buffer::findMemoryType(const VkMemoryRequirements& requirements,
                            VkMemoryPropertyFlags property_flags) const
    -> uint32_t {
  auto properties = logical_device_->physicalDevice()->memoryProperties();
  for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
    if ((requirements.memoryTypeBits & (1 << i)) &&
        (properties.memoryTypes[i].propertyFlags & property_flags) ==
            property_flags) {
      return i;
    }
  }
  throw std::runtime_error("Failed to find suitable memory type!");
}

// auto Buffer::copy(const CommandPoolPtr& command_pool, const BufferPtr&
// source,
//                   VkDeviceSize size) const -> void {
//   CommandBuffer command_buffer(logical_device_, command_pool);
//   command_buffer.copy();
//   command_buffer.record(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

//   VkBufferCopy copyRegion{};
//   copyRegion.size = size;
//   vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

//   vkEndCommandBuffer(commandBuffer);

//   VkSubmitInfo submitInfo{};
//   submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//   submitInfo.commandBufferCount = 1;
//   submitInfo.pCommandBuffers = &commandBuffer;

//   vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
//   vkQueueWaitIdle(graphicsQueue);

//   vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
// }

Buffer::Buffer(LogicalDevicePtr logical_device, VkDeviceSize size,
               VkBufferUsageFlags usage, VkMemoryPropertyFlags property_flags)
    : logical_device_(std::move(logical_device)), size_(size) {
  VkBufferCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  create_info.size = size_;
  create_info.usage = usage;
  create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(logical_device_->handle(), &create_info, nullptr,
                     &buffer_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create vertex buffer!");
  }

  auto requirements = memoryRequirements();
  VkMemoryAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = requirements.size;
  alloc_info.memoryTypeIndex = findMemoryType(requirements, property_flags);
  if (vkAllocateMemory(logical_device_->handle(), &alloc_info, nullptr,
                       &memory_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate vertex buffer memory!");
  }
  vkBindBufferMemory(logical_device_->handle(), buffer_, memory_, 0);
}
}  // namespace tupi
