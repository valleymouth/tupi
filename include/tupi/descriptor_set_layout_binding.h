#ifndef TUPI_DESCRIPTOR_SET_LAYOUT_BINDING
#define TUPI_DESCRIPTOR_SET_LAYOUT_BINDING

#include <variant>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"

namespace tupi {
struct DescriptorSetLayoutBinding : public VkDescriptorSetLayoutBinding {};
struct BindlessDescriptorSetLayoutBinding
    : public VkDescriptorSetLayoutBinding {};

using DescriptorSetLayoutBindingVec =
    std::vector<std::variant<DescriptorSetLayoutBinding,
                             BindlessDescriptorSetLayoutBinding>>;
}  // namespace tupi
#endif  // TUPI_DESCRIPTOR_SET_LAYOUT_BINDING