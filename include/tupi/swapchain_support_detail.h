#pragma once

#include <optional>
#include <ranges>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class SwapchainSupportDetail {
 public:
  SwapchainSupportDetail() = default;
  SwapchainSupportDetail(PhysicalDeviceSharedPtr physical_device,
                         ISurfacePtr surface);

  auto physicalDevice() const -> PhysicalDeviceSharedPtr {
    return physical_device_;
  }
  auto surface() const -> ISurfacePtr { return surface_; }
  auto surfaceCapabilities() const -> VkSurfaceCapabilitiesKHR {
    return capabilities_;
  }
  auto surfaceFormats() const -> SurfaceFormatVec { return formats_; }
  auto presentModes() const -> PresentModeVec { return present_modes_; }
  auto hasSurfaceFormat() const -> bool { return !formats_.empty(); }
  auto hasPresentMode() const -> bool { return !present_modes_.empty(); }
  auto surfaceFormat() const -> std::optional<SurfaceFormat>;
  auto presentMode() const -> std::optional<PresentMode>;
  auto swapchainExtent(uint32_t width, uint32_t height) const -> VkExtent2D;
  auto updateSurfaceCapabilities() -> void;

 private:
  PhysicalDeviceSharedPtr physical_device_{};
  ISurfacePtr surface_{};
  VkSurfaceCapabilitiesKHR capabilities_{};
  SurfaceFormatVec formats_{};
  PresentModeVec present_modes_{};
};

inline auto hasSurfaceFormat() {
  return std::views::filter([](const auto& x) { return x.hasSurfaceFormat(); });
}

inline auto hasPresentMode() {
  return std::views::filter([](const auto& x) { return x.hasPresentMode(); });
}
}  // namespace tupi
