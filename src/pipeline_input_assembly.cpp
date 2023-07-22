#include "tupi/pipeline_input_assembly.h"

namespace tupi {
PipelineInputAssembly::PipelineInputAssembly() {
  pipeline_create_info_.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  pipeline_create_info_.flags = 0;
  pipeline_create_info_.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  pipeline_create_info_.primitiveRestartEnable = VK_FALSE;
}
}  // namespace tupi
