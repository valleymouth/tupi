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
  SwapchainSupportDetail(PhysicalDevicePtr physical_device,
                         ISurfacePtr surface);

  auto physicalDevice() const -> PhysicalDevicePtr;
  auto surface() const -> ISurfacePtr;
  auto surfaceCapabilities() const -> VkSurfaceCapabilitiesKHR;
  auto surfaceFormats() const -> SurfaceFormatVec;
  auto presentModes() const -> PresentModeVec;
  auto hasSurfaceFormat() const -> bool;
  auto hasPresentMode() const -> bool;
  auto surfaceFormat() const -> std::optional<SurfaceFormat>;
  auto presentMode() const -> std::optional<PresentMode>;
  auto swapchainExtent(uint32_t width, uint32_t height) const -> VkExtent2D;
  auto updateSurfaceCapabilities() -> void;

 private:
  PhysicalDevicePtr physical_device_{};
  ISurfacePtr surface_{};
  VkSurfaceCapabilitiesKHR capabilities_{};
  SurfaceFormatVec formats_{};
  PresentModeVec present_modes_{};
};

inline auto SwapchainSupportDetail::physicalDevice() const
    -> PhysicalDevicePtr {
  return physical_device_;
}

inline auto SwapchainSupportDetail::surface() const -> ISurfacePtr {
  return surface_;
}

inline auto SwapchainSupportDetail::surfaceCapabilities() const
    -> VkSurfaceCapabilitiesKHR {
  return capabilities_;
}

inline auto SwapchainSupportDetail::surfaceFormats() const -> SurfaceFormatVec {
  return formats_;
}

inline auto SwapchainSupportDetail::presentModes() const -> PresentModeVec {
  return present_modes_;
}

inline auto SwapchainSupportDetail::hasSurfaceFormat() const -> bool {
  return !formats_.empty();
}

inline auto SwapchainSupportDetail::hasPresentMode() const -> bool {
  return !present_modes_.empty();
}

inline auto hasSurfaceFormat() {
  return std::views::filter([](const auto& x) { return x.hasSurfaceFormat(); });
}

inline auto hasPresentMode() {
  return std::views::filter([](const auto& x) { return x.hasPresentMode(); });
}
}  // namespace tupi
