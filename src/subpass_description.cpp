#include "tupi/subpass_description.h"

namespace tupi {
SubpassDescription::SubpassDescription(AttachmentReferenceVec color_attachments)
    : color_attachments_(std::move(color_attachments)) {
  subpass_.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass_.colorAttachmentCount = color_attachments_.size();
  subpass_.pColorAttachments = color_attachments_.data();
}
}  // namespace tupi
