#pragma once

#include <filesystem>
#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class Shader {
 public:
  enum Stage { Vertex, Fragment };

  Shader(LogicalDeviceSharedPtr logical_device,
         const std::filesystem::path& path, Stage stage);
  ~Shader();
  Shader(const Shader&) = delete;
  Shader(Shader&&) = delete;
  Shader& operator=(const Shader&) = default;
  Shader& operator=(Shader&&) = default;

  auto pipelineCreateInfo() const -> VkPipelineShaderStageCreateInfo {
    return pipeline_create_info_;
  }

  static auto pipelineCreateInfos(const ShaderPtrVec& shaders)
      -> std::vector<VkPipelineShaderStageCreateInfo>;

 private:
  LogicalDeviceSharedPtr logical_device_{};
  Handle<VkShaderModule> shader_{};
  VkPipelineShaderStageCreateInfo pipeline_create_info_{};
};
}  // namespace tupi
