#include "tupi/shader.h"

#include <fstream>

#include "tupi/logical_device.h"

namespace tupi {
Shader::Shader(LogicalDeviceSharedPtr logical_device,
               const std::filesystem::path& path, Shader::Stage stage)
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
  switch (stage) {
    case Stage::Vertex:
      pipeline_create_info_.stage = VK_SHADER_STAGE_VERTEX_BIT;
      break;
    case Stage::Fragment:
      pipeline_create_info_.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      break;
  }
  pipeline_create_info_.module = shader_;
  pipeline_create_info_.pName = "main";
}

Shader::~Shader() {
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
}  // namespace tupi
