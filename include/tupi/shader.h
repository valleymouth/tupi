#pragma once

#include <filesystem>
#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/resource.h"

namespace tupi {
class Shader : public internal::SharedResource<Shader> {
 public:
  enum Stage { Vertex, Fragment };

  Shader(Token, LogicalDevicePtr logical_device,
         const std::filesystem::path& path, Stage stage);
  ~Shader();

  auto pipelineCreateInfo() const -> VkPipelineShaderStageCreateInfo;

  static auto pipelineCreateInfos(const ShaderPtrVec& shaders)
      -> std::vector<VkPipelineShaderStageCreateInfo>;

 protected:
  Shader(const Shader&) = delete;
  Shader(Shader&&) = delete;
  Shader& operator=(const Shader&) = delete;
  Shader& operator=(Shader&&) = delete;

 private:
  LogicalDevicePtr logical_device_{};
  VkShaderModule shader_{};
  VkPipelineShaderStageCreateInfo pipeline_create_info_{};
};

inline auto Shader::pipelineCreateInfo() const
    -> VkPipelineShaderStageCreateInfo {
  return pipeline_create_info_;
}
}  // namespace tupi
