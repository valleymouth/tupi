#include "tupi/swapchain_support_detail.h"

#include <limits>

#include "tupi/physical_device.h"
#include "tupi/surface.h"

namespace tupi {
SwapchainSupportDetail::SwapchainSupportDetail(
    PhysicalDeviceSharedPtr physical_device, ISurfacePtr surface)
    : physical_device_(std::move(physical_device)),
      surface_(std::move(surface)) {
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device_->handle(),
                                            surface_->handle(), &capabilities_);
  uint32_t format_count = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(
      physical_device_->handle(), surface_->handle(), &format_count, nullptr);
  if (format_count != 0) {
    formats_.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device_->handle(),
                                         surface_->handle(), &format_count,
                                         formats_.data());
  }
  uint32_t present_mode_count = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device_->handle(),
                                            surface_->handle(),
                                            &present_mode_count, nullptr);
  if (present_mode_count != 0) {
    present_modes_.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        physical_device_->handle(), surface_->handle(), &present_mode_count,
        present_modes_.data());
  }
}

auto SwapchainSupportDetail::surfaceFormat() const
    -> std::optional<VkSurfaceFormatKHR> {
  if (formats_.empty()) {
    return {};
  }
  for (const auto& format : formats_) {
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
        format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }
  return formats_.at(0);
}

auto SwapchainSupportDetail::presentMode() const
    -> std::optional<VkPresentModeKHR> {
  if (present_modes_.empty()) {
    return {};
  }
  for (const auto& present_mode : present_modes_) {
    if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return present_mode;
    }
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

auto SwapchainSupportDetail::swapchainExtent(uint32_t width,
                                             uint32_t height) const
    -> VkExtent2D {
  if (capabilities_.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities_.currentExtent;
  } else {
    VkExtent2D extent = {static_cast<uint32_t>(width),
                         static_cast<uint32_t>(height)};
    extent.width = std::clamp(extent.width, capabilities_.minImageExtent.width,
                              capabilities_.maxImageExtent.width);
    extent.height =
        std::clamp(extent.height, capabilities_.minImageExtent.height,
                   capabilities_.maxImageExtent.height);
    return extent;
  }
}

auto SwapchainSupportDetail::updateSurfaceCapabilities() -> void {
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device_->handle(),
                                            surface_->handle(), &capabilities_);
}
}  // namespace tupi
