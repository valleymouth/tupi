#pragma once

#include <span>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/logical_device.h"

namespace tupi {
struct DescriptorSetLayoutInfo {
  bool bindless{false};
  DescriptorSetLayoutBindingVec bindings{};

  auto merge(const DescriptorSetLayoutInfo& other) -> void;
};

struct DescriptorSetLayoutConstructor {
  virtual VkDescriptorSetLayout construct(
      const LogicalDeviceSharedPtr& logical_device,
      const DescriptorSetLayoutBindingVec& bindings) const;
};

struct DescriptorSetLayoutBindlessConstructor
    : public DescriptorSetLayoutConstructor {
  virtual VkDescriptorSetLayout construct(
      const LogicalDeviceSharedPtr& logical_device,
      const DescriptorSetLayoutBindingVec& bindings) const override;
};

class DescriptorSetLayout {
 public:
  // Construct an empty descriptor set layout. This is needed if you are setting
  // a pipeline layout with a gap in the descriptor set indices. Let's say your
  // shaders are using just set = 0 and set = 2, you will need to include a
  // dummy descriptor set layout in index 1.
  explicit DescriptorSetLayout(LogicalDeviceSharedPtr logical_device);
  explicit DescriptorSetLayout(
      LogicalDeviceSharedPtr logical_device,
      DescriptorSetLayoutBindingVec bindings,
      const DescriptorSetLayoutConstructor& constructor =
          DescriptorSetLayoutConstructor{});
  virtual ~DescriptorSetLayout();
  DescriptorSetLayout(const DescriptorSetLayout&) = delete;
  DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
  DescriptorSetLayout(DescriptorSetLayout&& other) = default;
  DescriptorSetLayout& operator=(DescriptorSetLayout&& other) = default;

  auto handle() const -> VkDescriptorSetLayout {
    return descriptor_set_layout_;
  }

 private:
  LogicalDeviceSharedPtr logical_device_{};
  DescriptorSetLayoutBindingVec bindings_{};
  Handle<VkDescriptorSetLayout> descriptor_set_layout_{};
};

class BindlessDescriptorSetLayout : public DescriptorSetLayout {
 public:
  explicit BindlessDescriptorSetLayout(LogicalDeviceSharedPtr logical_device,
                                       DescriptorSetLayoutBindingVec bindings)
      : DescriptorSetLayout(std::move(logical_device), std::move(bindings),
                            DescriptorSetLayoutBindlessConstructor{}) {}
};
}  // namespace tupi
