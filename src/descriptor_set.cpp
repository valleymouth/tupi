#include "tupi/descriptor_set.h"

#include "tupi/buffer.h"
#include "tupi/descriptor_pool.h"
#include "tupi/descriptor_set_layout_binding.h"
#include "tupi/logical_device.h"

namespace tupi {
auto DescriptorSet::update(const BufferPtr buffer) -> void {
  VkDescriptorBufferInfo buffer_info{};
  buffer_info.buffer = buffer->handle();
  buffer_info.offset = 0;
  buffer_info.range = buffer->size();

  VkWriteDescriptorSet write{};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.dstSet = descriptor_set_;
  write.dstBinding = 0;
  write.dstArrayElement = 0;
  write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  write.descriptorCount = 1;
  write.pBufferInfo = &buffer_info;
  write.pImageInfo = nullptr;        // Optional
  write.pTexelBufferView = nullptr;  // Optional

  vkUpdateDescriptorSets(descriptor_pool_->logicalDevice()->handle(), 1, &write,
                         0, nullptr);
}

auto DescriptorSet::create(DescriptorPoolPtr descriptor_pool,
                           DescriptorSetLayoutPtrVec descriptor_set_layouts,
                           BufferPtrVec buffers) -> DescriptorSetPtrVec {
  VkDescriptorSetAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  alloc_info.descriptorPool = descriptor_pool->handle();
  alloc_info.descriptorSetCount =
      static_cast<uint32_t>(descriptor_set_layouts.size());
  auto vk_descriptor_set_layouts =
      DescriptorSetLayout::handles(descriptor_set_layouts);
  alloc_info.pSetLayouts = vk_descriptor_set_layouts.data();
  std::vector<VkDescriptorSet> vk_descriptor_sets;
  vk_descriptor_sets.resize(descriptor_set_layouts.size());
  auto logical_device = descriptor_pool->logicalDevice()->handle();
  if (vkAllocateDescriptorSets(logical_device, &alloc_info,
                               vk_descriptor_sets.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate descriptor sets!");
  }

  DescriptorSetPtrVec result;
  result.reserve(vk_descriptor_sets.size());
  int index = 0;
  for (const auto& vk_descriptor_set : vk_descriptor_sets) {
    const auto& buffer = buffers.at(index);
    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer = buffer->handle();
    buffer_info.offset = 0;
    buffer_info.range = buffer->size();

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = vk_descriptor_set;
    write.dstBinding = 0;
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write.descriptorCount = 1;
    write.pBufferInfo = &buffer_info;
    write.pImageInfo = nullptr;        // Optional
    write.pTexelBufferView = nullptr;  // Optional

    vkUpdateDescriptorSets(logical_device, 1, &write, 0, nullptr);

    DescriptorSetPtr descriptor_set = DescriptorSetPtr{new DescriptorSet()};
    descriptor_set->descriptor_pool_ = descriptor_pool;
    descriptor_set->descriptor_set_layout_ =
        std::move(descriptor_set_layouts.at(index));
    descriptor_set->buffer_ = buffer;
    descriptor_set->descriptor_set_ = vk_descriptor_set;
    result.emplace_back(std::move(descriptor_set));
    ++index;
  }
  return result;
}

auto DescriptorSet::handles(const DescriptorSetPtrVec& descriptor_sets)
    -> std::vector<VkDescriptorSet> {
  std::vector<VkDescriptorSet> result;
  result.reserve(descriptor_sets.size());
  for (const auto& descriptor_set : descriptor_sets) {
    result.emplace_back(descriptor_set->handle());
  }
  return result;
}

}  // namespace tupi
