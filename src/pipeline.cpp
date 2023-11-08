#include "tupi/pipeline.h"

#include "tupi/logical_device.h"
#include "tupi/pipeline_layout.h"
#include "tupi/render_pass.h"
#include "tupi/shader.h"

namespace tupi {
Pipeline::Pipeline(Token, LogicalDevicePtr logical_device, ShaderPtrVec shaders,
                   PipelineVertexInput vertex_input,
                   PipelineInputAssembly input_assembly,
                   PipelineViewportState viewport_state,
                   PipelineRasterizationState rasterization_state,
                   PipelineMultisampleState multisample_state,
                   PipelineColorBlendState color_blend_state,
                   PipelineDepthStencilState depth_stencil_state,
                   PipelineDynamicState dynamic_state,
                   PipelineLayoutPtr pipeline_layout, RenderPassPtr render_pass)
    : logical_device_(std::move(logical_device)),
      shaders_(std::move(shaders)),
      vertex_input_(std::move(vertex_input)),
      input_assembly_(std::move(input_assembly)),
      viewport_state_(std::move(viewport_state)),
      rasterization_state_(std::move(rasterization_state)),
      multisample_state_(std::move(multisample_state)),
      color_blend_state_(std::move(color_blend_state)),
      depth_stencil_state_(std::move(depth_stencil_state)),
      dynamic_state_(std::move(dynamic_state)),
      pipeline_layout_(std::move(pipeline_layout)),
      render_pass_(std::move(render_pass)) {
  VkGraphicsPipelineCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  create_info.stageCount = shaders_.size();
  auto vk_shaders = Shader::pipelineCreateInfos(shaders_);
  create_info.pStages = vk_shaders.data();
  auto vk_vertex_input = vertex_input_.pipelineCreateInfo();
  create_info.pVertexInputState = &vk_vertex_input;
  auto vk_input_assembly = input_assembly_.pipelineCreateInfo();
  create_info.pInputAssemblyState = &vk_input_assembly;
  auto vk_viewport_state = viewport_state_.pipelineCreateInfo();
  create_info.pViewportState = &vk_viewport_state;
  auto vk_rasterization_state = rasterization_state_.pipelineCreateInfo();
  create_info.pRasterizationState = &vk_rasterization_state;
  auto vk_multisample_state = multisample_state_.pipelineCreateInfo();
  create_info.pMultisampleState = &vk_multisample_state;
  create_info.pDepthStencilState = nullptr;  // Optional
  auto vk_color_blend_state = color_blend_state_.pipelineCreateInfo();
  create_info.pColorBlendState = &vk_color_blend_state;
  auto vk_depth_stencil_state = depth_stencil_state_.pipelineCreateInfo();
  create_info.pDepthStencilState = &vk_depth_stencil_state;
  auto vk_dynamic_state = dynamic_state_.pipelineCreateInfo();
  create_info.pDynamicState = &vk_dynamic_state;
  create_info.layout = pipeline_layout_->handle();
  create_info.renderPass = render_pass_->handle();
  create_info.subpass = 0;
  create_info.basePipelineHandle = VK_NULL_HANDLE;  // Optional
  create_info.basePipelineIndex = -1;               // Optional

  if (vkCreateGraphicsPipelines(logical_device_->handle(), VK_NULL_HANDLE, 1,
                                &create_info, nullptr,
                                &pipeline_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create graphics pipeline!");
  }
}

Pipeline::~Pipeline() {
  vkDestroyPipeline(logical_device_->handle(), pipeline_, nullptr);
}
}  // namespace tupi
