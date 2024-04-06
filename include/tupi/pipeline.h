#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"
#include "tupi/pipeline_color_blend_state.h"
#include "tupi/pipeline_depth_stencil_state.h"
#include "tupi/pipeline_dynamic_state.h"
#include "tupi/pipeline_input_assembly.h"
#include "tupi/pipeline_layout.h"
#include "tupi/pipeline_multisample_state.h"
#include "tupi/pipeline_rasterization_state.h"
#include "tupi/pipeline_vertex_input.h"
#include "tupi/pipeline_viewport_state.h"

namespace tupi {
class Pipeline {
 public:
  Pipeline(LogicalDeviceSharedPtr logical_device, ShaderPtrVec shaders,
           PipelineVertexInput vertex_input,
           PipelineInputAssembly input_assembly,
           PipelineViewportState viewport_state,
           PipelineRasterizationState rasterization_state,
           PipelineMultisampleState multisample_state,
           PipelineColorBlendState color_blend_state,
           PipelineDepthStencilState depth_stencil_state,
           PipelineDynamicState dynamic_state,
           PipelineLayoutPtr pipeline_layout, RenderPassPtr render_pass);
  ~Pipeline();
  Pipeline(const Pipeline&) = delete;
  Pipeline& operator=(const Pipeline&) = delete;
  Pipeline(Pipeline&&) = default;
  Pipeline& operator=(Pipeline&&) = default;

  auto handle() const -> VkPipeline { return pipeline_; }
  auto pipelineLayout() const -> PipelineLayoutPtr { return pipeline_layout_; }
  auto renderPass() const -> RenderPassPtr { return render_pass_; }

 protected:
 private:
  LogicalDeviceSharedPtr logical_device_{};
  ShaderPtrVec shaders_{};
  PipelineVertexInput vertex_input_{};
  PipelineInputAssembly input_assembly_{};
  PipelineViewportState viewport_state_{};
  PipelineRasterizationState rasterization_state_{};
  PipelineMultisampleState multisample_state_{};
  PipelineColorBlendState color_blend_state_{};
  PipelineDepthStencilState depth_stencil_state_{};
  PipelineDynamicState dynamic_state_{};
  PipelineLayoutPtr pipeline_layout_{};
  RenderPassPtr render_pass_{};
  Handle<VkPipeline> pipeline_{};
};
}  // namespace tupi
