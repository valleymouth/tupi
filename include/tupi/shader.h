#ifndef TUPI_SHADER_H
#define TUPI_SHADER_H

#include <spirv_reflect.h>

#include <filesystem>
#include <map>
#include <memory>
#include <span>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/pipeline_vertex_input.h"

class SpvReflectShaderModule;

namespace tupi {
constexpr uint32_t MAX_BINDLESS_RESOURCES = 1024;

class Shader {
  friend class Shader;

 public:
  Shader(LogicalDeviceSharedPtr logical_device,
         const std::filesystem::path& path);
  ~Shader();
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;
  Shader(Shader&&) = default;
  Shader& operator=(Shader&&) = default;

  auto stage() const -> VkShaderStageFlagBits {
    return pipeline_create_info_.stage;
  }

  auto pipelineCreateInfo() const -> VkPipelineShaderStageCreateInfo {
    return pipeline_create_info_;
  }

  auto vertexInputPipelineCreateInfo() const
      -> VkPipelineVertexInputStateCreateInfo {
    return pipeline_vertex_input_.pipelineCreateInfo();
  }

  static auto pipelineCreateInfos(const ShaderSharedPtrVec& shaders)
      -> std::vector<VkPipelineShaderStageCreateInfo>;

  static auto createPipelineLayout(LogicalDeviceSharedPtr logical_device,
                                   const ShaderSharedPtrVec& shaders)
      -> PipelineLayoutSharedPtr;

 private:
  using DescriptorSetLayoutInfoMap =
      std::map<uint32_t, DescriptorSetLayoutInfo>;

  LogicalDeviceSharedPtr logical_device_{};
  Handle<VkShaderModule> shader_{};
  VkPipelineShaderStageCreateInfo pipeline_create_info_{};
  SpvReflectShaderModule module_{};
  PipelineVertexInput pipeline_vertex_input_{};
  DescriptorSetLayoutInfoMap descriptor_set_layouts_{};
};
}  // namespace tupi
#endif  // TUPI_SHADER_H