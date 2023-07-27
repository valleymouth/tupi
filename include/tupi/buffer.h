#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"
#include "tupi/logical_device.h"

namespace tupi {
class Buffer {  //: public internal::Creatable<Buffer, std::shared_ptr> {
                // friend class internal::Creatable<Buffer, std::shared_ptr>;

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
  // auto copy(const CommandPoolPtr& command_pool, const BufferPtr& source,
  //           VkDeviceSize size) const -> void;

  template <typename T>
  static auto create(LogicalDevicePtr logical_device, uint32_t size,
                     VkBufferUsageFlags usage,
                     VkMemoryPropertyFlags property_flags) -> BufferPtr {
    return BufferPtr(new Buffer(std::move(logical_device), sizeof(T) * size,
                                usage, property_flags));
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
