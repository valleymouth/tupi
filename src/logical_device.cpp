#include "tupi/logical_device.h"

#include "tupi/physical_device.h"
#include "tupi/queue_family.h"

namespace tupi {
LogicalDevice::LogicalDevice(PhysicalDeviceSharedPtr physical_device,
                             const QueueCreateInfoVec& queue_create_infos,
                             const ExtensionSet& extensions)
    : physical_device_(std::move(physical_device)) {
  std::vector<VkDeviceQueueCreateInfo> vk_queue_create_infos;
  vk_queue_create_infos.reserve(queue_create_infos.size());
  for (const auto& queue_create_info : queue_create_infos) {
    VkDeviceQueueCreateInfo vk_queue_create_info{
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    vk_queue_create_info.queueFamilyIndex =
        queue_create_info.queue_family.index();
    vk_queue_create_info.queueCount = queue_create_info.priorities.size();
    vk_queue_create_info.pQueuePriorities = queue_create_info.priorities.data();
    vk_queue_create_infos.emplace_back(vk_queue_create_info);
  }

  const std::vector<const char*> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  VkDeviceCreateInfo create_info{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
  create_info.pQueueCreateInfos = vk_queue_create_infos.data();
  create_info.queueCreateInfoCount = vk_queue_create_infos.size();
  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  auto extension_names = extensions.toCStringVector();
  create_info.ppEnabledExtensionNames = extension_names.data();
  // Features including bindless support.
  // TODO: How do I expose this?
  VkPhysicalDeviceFeatures2 physical_features = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
  vkGetPhysicalDeviceFeatures2(physical_device_->handle(), &physical_features);
  create_info.pNext = &physical_features;
  auto indexing_features = physical_device_->indexingFeatures();
  if (physical_device_->hasBindlessSupport()) {
    physical_features.pNext = &indexing_features;
  }

  if (vkCreateDevice(physical_device_->handle(), &create_info, nullptr,
                     &device_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create logical device!");
  }
}

LogicalDevice::~LogicalDevice() { vkDestroyDevice(device_, nullptr); }

auto LogicalDevice::findMemoryType(const VkMemoryRequirements& requirements,
                                   VkMemoryPropertyFlags property_flags) const
    -> uint32_t {
  auto properties = physicalDevice()->memoryProperties();
  for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
    if ((requirements.memoryTypeBits & (1 << i)) &&
        (properties.memoryTypes[i].propertyFlags & property_flags) ==
            property_flags) {
      return i;
    }
  }
  throw std::runtime_error("Failed to find suitable memory type!");
}
}  // namespace tupi
