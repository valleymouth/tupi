#pragma once

#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/creatable.h"
#include "tupi/pipeline_color_blend_state.h"
#include "tupi/pipeline_dynamic_state.h"
#include "tupi/pipeline_input_assembly.h"
#include "tupi/pipeline_layout.h"
#include "tupi/pipeline_multisample_state.h"
#include "tupi/pipeline_rasterization_state.h"
#include "tupi/pipeline_vertex_input.h"
#include "tupi/pipeline_viewport_state.h"

namespace tupi {
class Pipeline : public internal::Creatable<Pipeline> {
  friend class internal::Creatable<Pipeline>;

 public:
  ~Pipeline();

  auto handle() const -> VkPipeline;
  auto renderPass() const -> RenderPassPtr;

 protected:
  Pipeline(LogicalDevicePtr logical_device, ShaderPtrVec shaders,
           PipelineVertexInput vertex_input,
           PipelineInputAssembly input_assembly,
           PipelineViewportState viewport_state,
           PipelineRasterizationState rasterization_state,
           PipelineMultisampleState multisample_state,
           PipelineColorBlendState color_blend_state,
           PipelineDynamicState dynamic_state, PipelineLayout pipeline_layout,
           RenderPassPtr render_pass);
  Pipeline(const Pipeline&) = delete;
  Pipeline(Pipeline&&) = delete;
  Pipeline& operator=(const Pipeline&) = delete;
  Pipeline& operator=(Pipeline&&) = delete;

 private:
  LogicalDevicePtr logical_device_{};
  ShaderPtrVec shaders_{};
  PipelineVertexInput vertex_input_{};
  PipelineInputAssembly input_assembly_{};
  PipelineViewportState viewport_state_{};
  PipelineRasterizationState rasterization_state_{};
  PipelineMultisampleState multisample_state_{};
  PipelineColorBlendState color_blend_state_{};
  PipelineDynamicState dynamic_state_{};
  PipelineLayout pipeline_layout_{};
  RenderPassPtr render_pass_{};
  VkPipeline pipeline_{};
};

inline auto Pipeline::handle() const -> VkPipeline { return pipeline_; }

inline auto Pipeline::renderPass() const -> RenderPassPtr {
  return render_pass_;
}
}  // namespace tupi