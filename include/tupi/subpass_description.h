#pragma once

#include <optional>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
class SubpassDescription {
 public:
  SubpassDescription() = default;
  SubpassDescription(AttachmentReferenceVec color_attachments);
  SubpassDescription(AttachmentReferenceVec color_attachments,
                     AttachmentReference depth_attachment);

  auto handle() const -> VkSubpassDescription { return subpass_; }
  auto hasDepth() const -> bool { return depth_attachment_.has_value(); }

  static auto handles(const SubpassDescriptionVec& subpasses)
      -> std::vector<VkSubpassDescription>;

 private:
  VkSubpassDescription subpass_{};
  AttachmentReferenceVec color_attachments_{};
  std::optional<AttachmentReference> depth_attachment_{};
};
}  // namespace tupi
