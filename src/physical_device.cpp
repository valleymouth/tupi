#include "tupi/physical_device.h"

#include "tupi/engine.h"

namespace tupi {
auto PhysicalDevice::enumerate(const EnginePtr& engine)
    -> PhysicalDevicePtrVec {
  uint32_t count = 0;
  vkEnumeratePhysicalDevices(engine->handle(), &count, nullptr);
  std::vector<VkPhysicalDevice> devices(count);
  vkEnumeratePhysicalDevices(engine->handle(), &count, devices.data());
  std::vector<PhysicalDevicePtr> result;
  result.reserve(count);
  for (const auto& vk_device : devices) {
    auto& device = result.emplace_back(PhysicalDevice::create());
    device->engine_ = engine;
    device->physical_device_ = vk_device;
    vkGetPhysicalDeviceProperties(vk_device, &device->properties_);
    vkGetPhysicalDeviceFeatures(vk_device, &device->features_);
    device->extensions_ = ExtensionSet::enumerate(device);
  }
  return result;
}
}  // namespace tupi
