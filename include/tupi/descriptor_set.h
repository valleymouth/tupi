#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/descriptor_set_layout.h"
#include "tupi/fwd.h"

namespace tupi {
class DescriptorSet {
 public:
  auto handle() const -> VkDescriptorSet;
  auto pool() const -> DescriptorPoolPtr;
  auto layout() const -> DescriptorSetLayoutPtr;
  auto update(const BufferPtr buffer) -> void;

  static auto create(DescriptorPoolPtr descriptor_pool,
                     DescriptorSetLayoutPtrVec descriptor_set_layouts,
                     BufferPtrVec buffers, ImageViewPtrVec image_views,
                     SamplerPtrVec samplers) -> DescriptorSetPtrVec;
  static auto handles(const DescriptorSetPtrVec& descriptor_sets)
      -> std::vector<VkDescriptorSet>;

 protected:
  DescriptorSet() = default;

 private:
  DescriptorPoolPtr descriptor_pool_{};
  DescriptorSetLayoutPtr descriptor_set_layout_{};
  BufferPtr buffer_{};
  VkDescriptorSet descriptor_set_{VK_NULL_HANDLE};
};

inline auto DescriptorSet::handle() const -> VkDescriptorSet {
  return descriptor_set_;
}
}  // namespace tupi
