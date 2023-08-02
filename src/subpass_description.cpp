#include "tupi/subpass_description.h"

namespace tupi {
SubpassDescription::SubpassDescription(AttachmentReferenceVec color_attachments)
    : color_attachments_(std::move(color_attachments)) {
  subpass_.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass_.colorAttachmentCount = color_attachments_.size();
  subpass_.pColorAttachments = color_attachments_.data();
}

auto SubpassDescription::handles(const SubpassDescriptionVec& subpasses)
    -> std::vector<VkSubpassDescription> {
  std::vector<VkSubpassDescription> result;
  result.reserve(subpasses.size());
  for (const auto& subpass : subpasses) {
    result.emplace_back(subpass.handle());
  }
  return result;
}
}  // namespace tupi
