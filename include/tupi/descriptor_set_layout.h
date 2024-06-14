#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/logical_device.h"

namespace tupi {
class DescriptorSetLayout {
 protected:
  struct Private {};

 public:
  explicit DescriptorSetLayout(Private) {}
  virtual ~DescriptorSetLayout();
  DescriptorSetLayout(const DescriptorSetLayout&) = delete;
  DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
  DescriptorSetLayout(DescriptorSetLayout&& other) = default;
  DescriptorSetLayout& operator=(DescriptorSetLayout&& other) = default;

  auto handle() const -> VkDescriptorSetLayout {
    return descriptor_set_layout_;
  }

  template <typename T>
  static auto create(LogicalDeviceSharedPtr logical_device,
                     DescriptorSetLayoutBindingVec bindings)
      -> DescriptorSetLayoutSharedPtr {
    VkDescriptorSetLayoutCreateInfo create_info{
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    auto binding_count = static_cast<uint32_t>(bindings.size());
    create_info.bindingCount = binding_count;
    create_info.pBindings = bindings.data();

    auto result = std::make_shared<T>(typename T::Private{});
    auto descriptor_set_layout =
        result->create(logical_device, bindings, create_info);
    result->logical_device_ = std::move(logical_device);
    result->bindings_ = std::move(bindings);
    result->descriptor_set_layout_ = descriptor_set_layout;
    return result;
  }

 protected:
  virtual auto create(const LogicalDeviceSharedPtr& logical_device,
                      const DescriptorSetLayoutBindingVec& bindings,
                      VkDescriptorSetLayoutCreateInfo& create_info)
      -> VkDescriptorSetLayout;

 private:
  LogicalDeviceSharedPtr logical_device_{};
  DescriptorSetLayoutBindingVec bindings_{};
  Handle<VkDescriptorSetLayout> descriptor_set_layout_{};
};

class BindlessDescriptorSetLayout : public DescriptorSetLayout {
  friend class DescriptorSetLayout;

 protected:
  struct Private {};

 public:
  explicit BindlessDescriptorSetLayout(Private)
      : DescriptorSetLayout(DescriptorSetLayout::Private{}) {}

  auto create(const LogicalDeviceSharedPtr& logical_device,
              const DescriptorSetLayoutBindingVec& bindings,
              VkDescriptorSetLayoutCreateInfo& create_info)
      -> VkDescriptorSetLayout override;
};
}  // namespace tupi
