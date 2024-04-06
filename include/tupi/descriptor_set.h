#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
/// @brief Remember: multiple descriptor sets are allocated and updated at once,
/// that is why the constructor is hidden by a private struct. They are also
/// destroyed when the descriptor pool is destroyed, so no need for a
/// destructor.
class DescriptorSet {
  struct Private {};

 public:
  DescriptorSet(Private){};

  auto handle() const -> VkDescriptorSet { return descriptor_set_; }

  auto pool() const -> DescriptorPoolSharedPtr;
  auto layout() const -> DescriptorSetLayoutSharedPtr;
  auto update(const BufferSharedPtr buffer) -> void;

  static auto create(DescriptorPoolSharedPtr descriptor_pool,
                     DescriptorSetLayoutSharedPtrVec descriptor_set_layouts,
                     BufferPtrVec buffers, ImageViewPtrVec image_views,
                     SamplerPtrVec samplers) -> DescriptorSetSharedPtrVec;

 private:
  DescriptorPoolSharedPtr descriptor_pool_{};
  DescriptorSetLayoutSharedPtr descriptor_set_layout_{};
  BufferSharedPtr buffer_{};
  Handle<VkDescriptorSet> descriptor_set_;
};
}  // namespace tupi
