#include "tupi/sampler.h"

#include "tupi/logical_device.h"
#include "tupi/physical_device.h"

namespace tupi {
Sampler::Sampler(LogicalDeviceSharedPtr logical_device, VkFilter mag_filter,
                 VkFilter min_filter, VkSamplerAddressMode address_mode_u,
                 VkSamplerAddressMode address_mode_t)
    : logical_device_(std::move(logical_device)) {
  VkSamplerCreateInfo info{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
  info.magFilter = mag_filter;
  info.minFilter = min_filter;
  info.addressModeU = address_mode_u;
  info.addressModeV = address_mode_t;
  info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  info.anisotropyEnable = VK_TRUE;
  info.maxAnisotropy =
      logical_device_->physicalDevice()->maxSamplerAnisotropy();
  info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  info.unnormalizedCoordinates = VK_FALSE;
  info.compareEnable = VK_FALSE;
  info.compareOp = VK_COMPARE_OP_ALWAYS;
  info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  info.mipLodBias = 0.0f;
  info.minLod = 0.0f;
  info.maxLod = 0.0f;

  if (vkCreateSampler(logical_device_->handle(), &info, nullptr, &sampler_) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create texture sampler!");
  }
}

Sampler::~Sampler() {
  vkDestroySampler(logical_device_->handle(), sampler_, nullptr);
}
}  // namespace tupi