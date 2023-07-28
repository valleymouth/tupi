#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/command_buffer.h"
#include "tupi/command_pool.h"
#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"
#include "tupi/logical_device.h"
#include "tupi/queue.h"

namespace tupi {
class Buffer {
 public:
  ~Buffer();

  auto handle() const -> VkBuffer;
  auto memoryRequirements() const -> VkMemoryRequirements;
  auto findMemoryType(const VkMemoryRequirements& requirements,
                      VkMemoryPropertyFlags property_flags) const -> uint32_t;

  template <typename T>
  auto copy(const std::vector<T>& vec) const -> void {
    void* data;
    vkMapMemory(logical_device_->handle(), memory_, 0, size_, 0, &data);
    memcpy(data, vec.data(), static_cast<std::size_t>(size_));
    vkUnmapMemory(logical_device_->handle(), memory_);
  }

  template <typename T>
  static auto create(LogicalDevicePtr logical_device, uint32_t size,
                     VkBufferUsageFlags usage,
                     VkMemoryPropertyFlags property_flags) -> BufferPtr {
    return BufferPtr(new Buffer(std::move(logical_device), sizeof(T) * size,
                                usage, property_flags));
  }

  template <typename T>
  static auto copy(const std::vector<T>& data, const BufferPtr& buffer,
                   const CommandPoolPtr& command_pool, const Queue& queue)
      -> void {
    const auto& logical_device = command_pool->logicalDevice();
    auto staging_buffer = tupi::Buffer::create<T>(
        logical_device, static_cast<uint32_t>(data.size()),
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    staging_buffer->copy(data);

    tupi::CommandBuffer command_buffer(logical_device, command_pool);
    command_buffer.copy(staging_buffer, buffer, sizeof(T) * data.size());
    command_buffer.record(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    queue.submit(command_buffer, {}, {}, {}, {}, true);
  }

 protected:
  Buffer(LogicalDevicePtr logical_device, VkDeviceSize size,
         VkBufferUsageFlags usage, VkMemoryPropertyFlags property_flags);
  Buffer(const Buffer&) = delete;
  Buffer(Buffer&&) = delete;
  auto operator=(const Buffer&) -> Buffer& = delete;
  auto operator=(Buffer&&) -> Buffer& = delete;

 private:
  LogicalDevicePtr logical_device_{};
  VkDeviceSize size_{};
  VkBuffer buffer_{VK_NULL_HANDLE};
  VkDeviceMemory memory_{VK_NULL_HANDLE};
};

inline auto Buffer::handle() const -> VkBuffer { return buffer_; }
}  // namespace tupi
