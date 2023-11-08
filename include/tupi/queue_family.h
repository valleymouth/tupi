#pragma once

#include <utility>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class QueueFamily {
 public:
  QueueFamily() = default;
  explicit QueueFamily(PhysicalDevicePtr physical_device)
      : physical_device_(std::move(physical_device)) {}

  auto index() const -> uint32_t;
  auto hasGraphics() const -> bool;
  auto hasPresentSupport(const ISurfacePtr& surface) const -> bool;

  static auto enumerate(const PhysicalDevicePtr& physical_device)
      -> QueueFamilyVec;

 private:
  PhysicalDevicePtr physical_device_{};
  uint32_t queue_family_index_{0};
  VkQueueFamilyProperties properties_{};
};

inline auto QueueFamily::index() const -> uint32_t {
  return queue_family_index_;
}

inline auto QueueFamily::hasGraphics() const -> bool {
  return properties_.queueFlags & VK_QUEUE_GRAPHICS_BIT;
}

static auto hasGraphics = [](const QueueFamily& queue_family) {
  return queue_family.hasGraphics();
};

struct hasPresentSupport {
  ISurfacePtr surface;

  auto operator()(const QueueFamily& queue_family) const -> bool {
    return queue_family.hasPresentSupport(surface);
  }
};
}  // namespace tupi
