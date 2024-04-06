#include "tupi/handle.h"

#include <iostream>

#include "tupi/engine.h"
#include "tupi/extension_set.h"
#include "tupi/physical_device.h"

auto main() -> int {
  {
    auto engine1 = tupi::Engine("Hello Triangle", VK_MAKE_VERSION(0, 1, 0),
                                tupi::ExtensionSet{});
    auto engine2 = std::move(engine1);
    if (engine1.handle() != VK_NULL_HANDLE) {
      std::cerr << "Error, expected VK_NULL_HANDLE!" << std::endl;
    }
    if (engine2.handle() == VK_NULL_HANDLE) {
      std::cerr << "Error, got VK_NULL_HANDLE!" << std::endl;
    }
  }
  auto engine = std::make_shared<tupi::Engine>(
      "Hello Triangle", VK_MAKE_VERSION(0, 1, 0), tupi::ExtensionSet{});

  auto physical_devices = tupi::PhysicalDevice::enumerate(engine);
  if (!physical_devices.empty()) {
    auto& physical_device1 = *physical_devices.at(0);
    auto physical_device2 = std::move(physical_device1);
  }
  return 0;
}
