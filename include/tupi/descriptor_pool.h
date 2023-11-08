#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/resource.h"

namespace tupi {
class DescriptorPool : public internal::SharedResource<DescriptorPool> {
 public:
  DescriptorPool(Token, LogicalDevicePtr logical_device,
                 DescriptorPoolSizeVec pool_sizes, uint32_t max_sets);
  ~DescriptorPool();

  auto handle() const -> VkDescriptorPool;
  auto logicalDevice() const -> LogicalDevicePtr;

 protected:
  DescriptorPool(const DescriptorPool&) = delete;
  DescriptorPool(DescriptorPool&&) = delete;
  auto operator=(const DescriptorPool&) -> DescriptorPool& = delete;
  auto operator=(DescriptorPool&&) -> DescriptorPool& = delete;

 private:
  LogicalDevicePtr logical_device_{};
  DescriptorPoolSizeVec pool_sizes_{};
  VkDescriptorPool descriptor_pool_{};
};

inline auto DescriptorPool::handle() const -> VkDescriptorPool {
  return descriptor_pool_;
}

inline auto DescriptorPool::logicalDevice() const -> LogicalDevicePtr {
  return logical_device_;
}
}  // namespace tupi
