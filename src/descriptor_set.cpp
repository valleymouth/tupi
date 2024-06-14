#include "tupi/descriptor_set.h"

#include <array>

#include "tupi/buffer.h"
#include "tupi/descriptor_pool.h"
#include "tupi/descriptor_set_layout.h"
#include "tupi/image_view.h"
#include "tupi/logical_device.h"
#include "tupi/sampler.h"
#include "tupi/texture.h"
#include "tupi/utility.h"

namespace tupi {
auto DescriptorSet::update(const LogicalDeviceSharedPtr& logical_device,
                           const WriteDescriptorSetVec& writes) -> void {
  using WriteVec = std::vector<VkWriteDescriptorSet>;
  using BufferInfoVec = std::vector<VkDescriptorBufferInfo>;
  using ImageInfoVec = std::vector<VkDescriptorImageInfo>;

  WriteVec vk_writes(writes.size(), {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET});
  BufferInfoVec vk_buffer_infos(writes.size());
  ImageInfoVec vk_image_infos(writes.size());
  size_t index = 0;
  for (const auto& write : writes) {
    std::visit(
        [&](auto&& resource) {
          using T = std::decay_t<decltype(resource)>;
          if constexpr (std::is_same_v<T, BufferSharedPtr>) {
            write.descriptor_set->update(write, vk_writes.at(index),
                                         vk_buffer_infos.at(index), resource);
          } else if constexpr (std::is_same_v<T, ITextureSharedPtr>) {
            write.descriptor_set->update(write, vk_writes.at(index),
                                         vk_image_infos.at(index), resource);
          } else {
            // static_assert(false,
            //               "Illegal resource type in DescriptorSet::update!");
          }
          ++index;
        },
        write.resource);
  }
  vkUpdateDescriptorSets(logical_device->handle(), writes.size(),
                         vk_writes.data(), 0, nullptr);
}

auto DescriptorSet::create(
    DescriptorPoolSharedPtr descriptor_pool,
    DescriptorSetLayoutSharedPtrVec descriptor_set_layouts)
    -> DescriptorSetSharedPtrVec {
  VkDescriptorSetAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  alloc_info.descriptorPool = descriptor_pool->handle();
  alloc_info.descriptorSetCount =
      static_cast<uint32_t>(descriptor_set_layouts.size());
  auto vk_descriptor_set_layouts = handles(descriptor_set_layouts);
  alloc_info.pSetLayouts = vk_descriptor_set_layouts.data();
  std::vector<VkDescriptorSet> vk_descriptor_sets;
  vk_descriptor_sets.resize(descriptor_set_layouts.size());
  auto logical_device = descriptor_pool->logicalDevice()->handle();
  if (vkAllocateDescriptorSets(logical_device, &alloc_info,
                               vk_descriptor_sets.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate descriptor sets!");
  }

  DescriptorSetSharedPtrVec result;
  result.reserve(vk_descriptor_sets.size());
  int index = 0;
  for (const auto& vk_descriptor_set : vk_descriptor_sets) {
    auto descriptor_set = std::make_shared<DescriptorSet>(Private{});
    descriptor_set->descriptor_pool_ = descriptor_pool;
    descriptor_set->descriptor_set_layout_ =
        std::move(descriptor_set_layouts.at(index));
    descriptor_set->descriptor_set_ = vk_descriptor_set;
    result.push_back(std::move(descriptor_set));
    ++index;
  }
  return result;
}

auto DescriptorSet::update(const WriteDescriptorSet& write,
                           VkWriteDescriptorSet& vk_write,
                           VkDescriptorBufferInfo& info,
                           const BufferSharedPtr& buffer) -> void {
  info.buffer = buffer->handle();
  info.offset = 0;
  info.range = buffer->size();

  vk_write.dstSet = descriptor_set_.handle;
  vk_write.dstBinding = write.binding;
  vk_write.dstArrayElement = write.array_element;
  if (buffer->isStorage()) {
    vk_write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  } else {
    vk_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  }
  vk_write.descriptorCount = 1;
  vk_write.pBufferInfo = &info;
  vk_write.pImageInfo = nullptr;        // Optional
  vk_write.pTexelBufferView = nullptr;  // Optional
}

auto DescriptorSet::update(const WriteDescriptorSet& write,
                           VkWriteDescriptorSet& vk_write,
                           VkDescriptorImageInfo& info,
                           const ITextureSharedPtr& texture) -> void {
  info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  info.imageView = texture->imageView()->handle();
  info.sampler = texture->sampler()->handle();

  vk_write.dstSet = descriptor_set_.handle;
  vk_write.dstBinding = write.binding;
  vk_write.dstArrayElement = write.array_element;
  vk_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  vk_write.descriptorCount = 1;
  vk_write.pImageInfo = &info;
  vk_write.pBufferInfo = nullptr;       // Optional
  vk_write.pTexelBufferView = nullptr;  // Optional
}
}  // namespace tupi
