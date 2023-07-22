#pragma once

#include <memory>
#include <tuple>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"
#include "tupi/queue_family.h"
#include "tupi/swapchain_support_detail.h"

namespace tupi {
class Swapchain : public internal::Creatable<Swapchain, std::shared_ptr> {
  friend class internal::Creatable<Swapchain, std::shared_ptr>;

 public:
  ~Swapchain();

  auto handle() const -> VkSwapchainKHR;
  auto logicalDevice() const -> LogicalDevicePtr;
  auto extent() const -> VkExtent2D;
  auto format() const -> VkFormat;
  auto images() const -> ImagePtrVec;
  auto acquireNextImage(const SemaphorePtr& semaphore) const
      -> std::tuple<VkResult, uint32_t>;

  static auto recreate(SwapchainPtr& swapchain) -> void;

 protected:
  Swapchain(LogicalDevicePtr logical_device,
            SwapchainSupportDetail swapchain_support_detail,
            const QueueFamily& graphics_queue_family,
            const QueueFamily& present_queue_family);
  Swapchain(const Swapchain&) = delete;
  Swapchain(Swapchain&&) = delete;
  Swapchain& operator=(const Swapchain&) = delete;
  Swapchain& operator=(Swapchain&&) = delete;

 private:
  LogicalDevicePtr logical_device_{};
  SwapchainSupportDetail swapchain_support_detail_{};
  QueueFamily graphics_queue_family_{};
  QueueFamily present_queue_family_{};
  VkSwapchainKHR swapchain_{VK_NULL_HANDLE};
  VkExtent2D extent_{};
  VkFormat format_{};
  ImagePtrVec images_{};
};

inline auto Swapchain::handle() const -> VkSwapchainKHR { return swapchain_; }

inline auto Swapchain::logicalDevice() const -> LogicalDevicePtr {
  return logical_device_;
}

inline auto Swapchain::extent() const -> VkExtent2D { return extent_; }

inline auto Swapchain::format() const -> VkFormat { return format_; }

inline auto Swapchain::images() const -> ImagePtrVec { return images_; }
}  // namespace tupi
