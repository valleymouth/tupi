#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/command_buffer.h"
#include "tupi/command_pool.h"
#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/logical_device.h"
#include "tupi/queue.h"

namespace tupi {
class Buffer {
 public:
  Buffer(LogicalDeviceSharedPtr logical_device, VkDeviceSize size,
         VkBufferUsageFlags usage, VkMemoryPropertyFlags property_flags);
  ~Buffer();
  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;
  Buffer(Buffer&& other);
  Buffer& operator=(Buffer&& other);

  auto handle() const -> VkBuffer { return buffer_; }
  auto size() const -> VkDeviceSize { return size_; }
  auto isMapped() const -> bool { return data_ != nullptr; }
  auto isStorage() const -> bool {
    return usage_ & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  }
  auto isUniform() const -> bool {
    return usage_ & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  }
  auto memoryRequirements() const -> VkMemoryRequirements;
  auto map() -> void;
  auto unmap() -> void;

  template <typename T>
  auto copy(const std::vector<T>& vec, bool keep_mapped = false) -> void {
    if (!isMapped()) {
      map();
    }
    memcpy(data_, vec.data(), static_cast<std::size_t>(size_));
    if (!keep_mapped) {
      unmap();
    }
  }

  template <typename T>
  auto copy(const T& x, bool keep_mapped = false) -> void {
    if (!isMapped()) {
      map();
    }
    memcpy(data_, &x, static_cast<std::size_t>(size_));
    if (!keep_mapped) {
      unmap();
    }
  }

  template <typename T>
  auto copy(T* data, bool keep_mapped = false) -> void {
    if (!isMapped()) {
      map();
    }
    memcpy(data_, data, static_cast<std::size_t>(size_));
    if (!keep_mapped) {
      unmap();
    }
  }

  template <typename T>
  static auto copy(const std::vector<T>& data, const BufferSharedPtr& buffer,
                   const CommandPoolSharedPtr& command_pool, const Queue& queue)
      -> void {
    const auto& logical_device = command_pool->logicalDevice();
    auto staging_buffer = tupi::Buffer::createShared<T>(
        logical_device, static_cast<uint32_t>(data.size()),
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    staging_buffer->copy(data);

    auto command_buffer = std::make_unique<CommandBuffer>(command_pool);
    command_buffer->copy(staging_buffer, buffer, sizeof(T) * data.size());
    command_buffer->record(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    queue.submitAndWaitIdle(command_buffer);
  }

  template <typename T>
  static auto createShared(LogicalDeviceSharedPtr logical_device, uint32_t size,
                           VkBufferUsageFlags usage,
                           VkMemoryPropertyFlags property_flags) {
    return std::make_shared<Buffer>(std::move(logical_device), sizeof(T) * size,
                                    usage, property_flags);
  }
  template <typename T>
  static auto createShared(LogicalDeviceSharedPtr logical_device,
                           VkBufferUsageFlags usage,
                           VkMemoryPropertyFlags property_flags) {
    return std::make_shared<Buffer>(std::move(logical_device), sizeof(T), usage,
                                    property_flags);
  }

  static auto handles(const BufferSharedPtrVec& buffers)
      -> std::vector<VkBuffer>;

 private:
  LogicalDeviceSharedPtr logical_device_{};
  VkDeviceSize size_{};
  VkBufferUsageFlags usage_{};
  Handle<VkBuffer> buffer_{};
  Handle<VkDeviceMemory> memory_{};
  void* data_{nullptr};
};
}  // namespace tupi
