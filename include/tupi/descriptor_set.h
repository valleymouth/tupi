#pragma once

#include <variant>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
using DescriptorSetResource = std::variant<BufferSharedPtr, ITextureSharedPtr>;
using DescriptorSetResourceVec = std::vector<DescriptorSetResource>;

struct WriteDescriptorSet {
  using ResourceType = std::variant<BufferSharedPtr, ITextureSharedPtr>;

  // explicit WriteDescriptorSet(BufferSharedPtr buffer)
  //     : resource(std::move(buffer)) {}
  // explicit WriteDescriptorSet(ITextureSharedPtr texture)
  //     : resource(std::move(texture)) {}

  DescriptorSetSharedPtr descriptor_set;
  ResourceType resource;
  uint32_t binding{0};
  uint32_t array_element{0};
};
using WriteDescriptorSetVec = std::vector<WriteDescriptorSet>;

/// @brief Remember: multiple descriptor sets are allocated and updated at once,
/// that is why the constructor is hidden by a private struct. They are also
/// destroyed when the descriptor pool is destroyed, so no need to define a
/// destructor.
class DescriptorSet {
  struct Private {};

 public:
  DescriptorSet(Private){};

  auto handle() const -> VkDescriptorSet { return descriptor_set_; }

  auto pool() const -> DescriptorPoolSharedPtr;
  auto layout() const -> DescriptorSetLayoutSharedPtr;

  static auto update(const LogicalDeviceSharedPtr& logical_device,
                     const WriteDescriptorSetVec& writes) -> void;
  static auto create(DescriptorPoolSharedPtr descriptor_pool,
                     DescriptorSetLayoutSharedPtrVec descriptor_set_layouts)
      -> DescriptorSetSharedPtrVec;

 private:
  auto update(const WriteDescriptorSet& write, VkWriteDescriptorSet& vk_write,
              VkDescriptorBufferInfo& info, const BufferSharedPtr& buffer)
      -> void;
  auto update(const WriteDescriptorSet& write, VkWriteDescriptorSet& vk_write,
              VkDescriptorImageInfo& info, const ITextureSharedPtr& texture)
      -> void;

  DescriptorPoolSharedPtr descriptor_pool_{};
  DescriptorSetLayoutSharedPtr descriptor_set_layout_{};
  Handle<VkDescriptorSet> descriptor_set_;
};
}  // namespace tupi
