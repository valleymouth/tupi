#include "tupi/descriptor_pool.h"

#include "tupi/logical_device.h"

namespace tupi {
DescriptorPool::~DescriptorPool() {
  vkDestroyDescriptorPool(logical_device_->handle(), descriptor_pool_, nullptr);
}

DescriptorPool::DescriptorPool(LogicalDevicePtr logical_device,
                               DescriptorPoolSizeVec pool_sizes,
                               uint32_t max_sets)
    : logical_device_(std::move(logical_device)),
      pool_sizes_(std::move(pool_sizes)) {
  VkDescriptorPoolCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  create_info.poolSizeCount = static_cast<uint32_t>(pool_sizes_.size());
  create_info.pPoolSizes = pool_sizes_.data();
  create_info.maxSets = max_sets;
  if (vkCreateDescriptorPool(logical_device_->handle(), &create_info, nullptr,
                             &descriptor_pool_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor pool!");
  }
}
}  // namespace tupi
