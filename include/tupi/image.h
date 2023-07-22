#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"

namespace tupi {
class Image : public internal::Creatable<Image, std::shared_ptr> {
  friend class internal::Creatable<Image, std::shared_ptr>;

 public:
  auto handle() const -> VkImage;

  static auto enumerate(const Swapchain& swapchain) -> ImagePtrVec;

 protected:
  Image() = default;

 private:
  VkImage image_{VK_NULL_HANDLE};
};

inline auto Image::handle() const -> VkImage { return image_; }
}  // namespace tupi
