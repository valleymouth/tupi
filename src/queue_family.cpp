#include "tupi/queue_family.h"

#include "tupi/physical_device.h"
#include "tupi/surface.h"

namespace tupi {
auto QueueFamily::hasPresentSupport(const ISurfacePtr& surface) const -> bool {
  VkBool32 present_support = false;
  vkGetPhysicalDeviceSurfaceSupportKHR(physical_device_->handle(),
                                       queue_family_index_, surface->handle(),
                                       &present_support);
  return present_support == VK_TRUE;
}

auto QueueFamily::enumerate(const PhysicalDeviceSharedPtr& physical_device)
    -> std::vector<QueueFamily> {
  uint32_t count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device->handle(), &count,
                                           nullptr);
  std::vector<VkQueueFamilyProperties> queue_families(count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device->handle(), &count,
                                           queue_families.data());
  std::vector<QueueFamily> result;
  result.reserve(count);
  uint32_t index = 0;
  for (const auto& vk_queue : queue_families) {
    auto& queue = result.emplace_back(physical_device);
    queue.queue_family_index_ = index++;
    queue.properties_ = vk_queue;
  }
  return result;
}
}  // namespace tupi
