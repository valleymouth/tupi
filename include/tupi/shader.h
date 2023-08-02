#pragma once

#include <filesystem>
#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"

namespace tupi {
class Shader : public internal::Creatable<Shader, std::shared_ptr> {
  friend class internal::Creatable<Shader, std::shared_ptr>;

 public:
  enum Stage { Vertex, Fragment };

  ~Shader();

  auto pipelineCreateInfo() const -> VkPipelineShaderStageCreateInfo;

  static auto pipelineCreateInfos(const ShaderPtrVec& shaders)
      -> std::vector<VkPipelineShaderStageCreateInfo>;

 protected:
  Shader(LogicalDevicePtr logical_device, const std::filesystem::path& path,
         Stage stage);
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
