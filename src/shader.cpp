#include "tupi/shader.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <set>

#include "tupi/descriptor_set_layout.h"
#include "tupi/logical_device.h"
#include "tupi/pipeline_layout.h"

namespace tupi {
static uint32_t formatSize(VkFormat format) {
  switch (format) {
    case VK_FORMAT_R32_UINT:
      return 4;
    case VK_FORMAT_R32G32_SFLOAT:
      return 8;
    case VK_FORMAT_R32G32B32_SFLOAT:
      return 12;
    case VK_FORMAT_R32G32B32A32_SFLOAT:
      return 16;
    default:
      throw std::runtime_error("Unsupported Vulkan format");
  }
}

bool containsCaseInsensitive(std::string_view haystack,
                             std::string_view needle) {
  if (needle.empty()) return true;
  auto subrange = std::ranges::search(haystack, needle, [](char a, char b) {
    return std::tolower(static_cast<unsigned char>(a)) ==
           std::tolower(static_cast<unsigned char>(b));
  });
  return !subrange.empty();
}

Shader::Shader(LogicalDeviceSharedPtr logical_device,
               const std::filesystem::path& path)
    : logical_device_(std::move(logical_device)) {
  if (!std::filesystem::exists(path)) {
    throw std::runtime_error("Can't find shader file: " + path.string());
  }

  std::ifstream file(path.string(), std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("Can't open shader file: " + path.string());
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);

  VkShaderModuleCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = buffer.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

  if (vkCreateShaderModule(logical_device_->handle(), &create_info, nullptr,
                           &shader_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create shader module!");
  }

  pipeline_create_info_.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

  SpvReflectResult result =
      spvReflectCreateShaderModule(fileSize, buffer.data(), &module_);
  if (result != SPV_REFLECT_RESULT_SUCCESS) {
    throw std::runtime_error("SPIRV-Reflect failed to read shader file!");
  }

  // TODO: use SPIRV-Reflect to create the pipeline layout.
  uint32_t input_variable_count = 0;
  result =
      spvReflectEnumerateInputVariables(&module_, &input_variable_count, NULL);
  if (result != SPV_REFLECT_RESULT_SUCCESS) {
    throw std::runtime_error("SPIRV-Reflect failed to read shader file!");
  }

  std::vector<SpvReflectInterfaceVariable*> input_variables(
      input_variable_count);
  result = spvReflectEnumerateInputVariables(&module_, &input_variable_count,
                                             input_variables.data());
  if (result != SPV_REFLECT_RESULT_SUCCESS) {
    throw std::runtime_error("SPIRV-Reflect failed to read shader file!");
  }

  VertexInputAttributeDescriptionVec attribute_descriptions;
  VertexInputBindingDescriptionVec binding_descriptions;
  switch (module_.shader_stage) {
    case SPV_REFLECT_SHADER_STAGE_VERTEX_BIT: {
      pipeline_create_info_.stage = VK_SHADER_STAGE_VERTEX_BIT;
      for (const auto variable : input_variables) {
        // Exclude build-in variables.
        if (!(variable->decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN)) {
          attribute_descriptions.push_back(
              {variable->location, 0, static_cast<VkFormat>(variable->format),
               0});
        }
      }
      std::ranges::sort(attribute_descriptions, {},
                        &VertexInputAttributeDescription::location);
      uint32_t offset = 0;
      for (auto& attribute_description : attribute_descriptions) {
        attribute_description.offset = offset;
        offset += formatSize(attribute_description.format);
      }
      binding_descriptions.push_back({0, offset, VK_VERTEX_INPUT_RATE_VERTEX});

      pipeline_vertex_input_ = PipelineVertexInput(
          std::move(binding_descriptions), std::move(attribute_descriptions));
      break;
    }
    case SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT:
      pipeline_create_info_.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      break;
  }

  uint32_t descriptor_set_count = 0;
  result =
      spvReflectEnumerateDescriptorSets(&module_, &descriptor_set_count, NULL);
  if (result != SPV_REFLECT_RESULT_SUCCESS) {
    throw std::runtime_error("SPIRV-Reflect failed to read shader file!");
  }

  std::vector<SpvReflectDescriptorSet*> descriptor_sets(descriptor_set_count);
  result = spvReflectEnumerateDescriptorSets(&module_, &descriptor_set_count,
                                             descriptor_sets.data());
  if (result != SPV_REFLECT_RESULT_SUCCESS) {
    throw std::runtime_error("SPIRV-Reflect failed to read shader file!");
  }

  std::set<uint32_t> bindless_bindings;
  for (const auto* descriptor_set : descriptor_sets) {
    bool bindless = false;
    DescriptorSetLayoutBindingVec bindings;
    DescriptorSetLayoutBinding binding{};
    for (auto i = 0; i < descriptor_set->binding_count; i++) {
      const auto* spv_binding = descriptor_set->bindings[i];
      if (containsCaseInsensitive(spv_binding->name, "bindless")) {
        if (bindless_bindings.contains(spv_binding->binding)) {
          // bindless alias, skip.
          break;
        }
        bindless = true;
        bindless_bindings.emplace(spv_binding->binding);
      }
      binding.binding = spv_binding->binding;
      binding.descriptorType =
          static_cast<VkDescriptorType>(spv_binding->descriptor_type);
      if (bindless) {
        binding.descriptorCount = MAX_BINDLESS_RESOURCES;
      } else {
        binding.descriptorCount = 1;
        // Gets the total number of elements in a multidimentional array.
        for (auto dim = 0; dim < spv_binding->array.dims_count; dim++) {
          binding.descriptorCount *= spv_binding->array.dims[dim];
        }
      }
      binding.stageFlags = module_.shader_stage;
      bindings.push_back(binding);
    }
    descriptor_set_layouts_[descriptor_set->set] = {bindless,
                                                    std::move(bindings)};
  }

  pipeline_create_info_.module = shader_;
  pipeline_create_info_.pName = "main";
}

Shader::~Shader() {
  spvReflectDestroyShaderModule(&module_);
  vkDestroyShaderModule(logical_device_->handle(), shader_, nullptr);
}

auto Shader::pipelineCreateInfos(const ShaderSharedPtrVec& shaders)
    -> std::vector<VkPipelineShaderStageCreateInfo> {
  std::vector<VkPipelineShaderStageCreateInfo> result;
  result.reserve(shaders.size());
  for (const auto& shader : shaders) {
    result.emplace_back(shader->pipelineCreateInfo());
  }
  return result;
}

auto Shader::createPipelineLayout(LogicalDeviceSharedPtr logical_device,
                                  const ShaderSharedPtrVec& shaders)
    -> PipelineLayoutSharedPtr {
  uint32_t max_set = 0;
  for (const auto& shader : shaders) {
    if (!shader->descriptor_set_layouts_.empty()) {
      max_set =
          std::max(max_set, shader->descriptor_set_layouts_.rbegin()->first);
    }
  }

  uint32_t set_count = max_set + 1;
  std::vector<DescriptorSetLayoutInfo> infos(set_count);
  for (uint32_t index = 0; index < set_count; index++) {
    bool is_set_used = false;
    for (const auto& shader : shaders) {
      if (shader->descriptor_set_layouts_.contains(index)) {
        if (is_set_used) {
          infos[index].merge(shader->descriptor_set_layouts_[index]);
        } else {
          is_set_used = true;
          infos[index] = shader->descriptor_set_layouts_[index];
        }
      }
    }
    if (!is_set_used) {  // Add dummy descriptor set layout.
      infos[index] = {};
    }
  }

  DescriptorSetLayoutSharedPtrVec descriptor_set_layouts;
  for (const auto& info : infos) {
    auto descriptor_set_layout =
        info.bindless ? std::make_shared<BindlessDescriptorSetLayout>(
                            logical_device, info.bindings)
                      : std::make_shared<DescriptorSetLayout>(logical_device,
                                                              info.bindings);
    descriptor_set_layouts.push_back(std::move(descriptor_set_layout));
  }

  return std::make_shared<PipelineLayout>(std::move(logical_device),
                                          std::move(descriptor_set_layouts));
}
}  // namespace tupi
