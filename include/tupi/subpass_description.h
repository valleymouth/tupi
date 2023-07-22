#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class SubpassDescription {
 public:
  SubpassDescription() = default;
  SubpassDescription(AttachmentReferenceVec color_attachments);

  auto handle() const -> VkSubpassDescription;

 private:
  VkSubpassDescription subpass_{};
  AttachmentReferenceVec color_attachments_{};
};

inline auto SubpassDescription::handle() const -> VkSubpassDescription {
  return subpass_;
}
}  // namespace tupi
