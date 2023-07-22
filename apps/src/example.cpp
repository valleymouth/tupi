#include <algorithm>
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "tupi/command_buffer.h"
#include "tupi/command_pool.h"
#include "tupi/engine.h"
#include "tupi/extension_set.h"
#include "tupi/fence.h"
#include "tupi/frame.h"
#include "tupi/framebuffer.h"
#include "tupi/glfw/surface.h"
#include "tupi/glfw/window.h"
#include "tupi/logical_device.h"
#include "tupi/physical_device.h"
#include "tupi/pipeline.h"
#include "tupi/pipeline_color_blend_state.h"
#include "tupi/pipeline_dynamic_state.h"
#include "tupi/pipeline_input_assembly.h"
#include "tupi/pipeline_layout.h"
#include "tupi/pipeline_multisample_state.h"
#include "tupi/pipeline_rasterization_state.h"
#include "tupi/pipeline_vertex_input.h"
#include "tupi/pipeline_viewport_state.h"
#include "tupi/queue.h"
#include "tupi/queue_family.h"
#include "tupi/render_pass.h"
#include "tupi/semaphore.h"
#include "tupi/shader.h"
#include "tupi/subpass_description.h"
#include "tupi/surface.h"
#include "tupi/swapchain.h"
#include "tupi/swapchain_support_detail.h"

bool checkValidationLayersSupport(
    const std::vector<const char*>& validation_layers) {
  uint32_t layer_count = 0;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const auto& layer_name : validation_layers) {
    bool layer_found = false;
    for (const auto& layerProperties : available_layers) {
      if (layer_name == std::string_view(layerProperties.layerName)) {
        layer_found = true;
        break;
      }
    }
    if (!layer_found) {
      return false;
    }
  }
  return true;
}

int main() {
  glfwInit();

  constexpr uint32_t WIDTH = 800;
  constexpr uint32_t HEIGHT = 600;
  auto window = tupi::glfw::Window::create(WIDTH, HEIGHT);

  // Need to ensure null terminated strings.
  const std::vector<const char*> validation_layers = {
      "VK_LAYER_KHRONOS_validation"};
  // #ifdef DEBUG
  constexpr bool ENABLE_VALIDATION_LAYERS = true;
  // #else
  // constexpr bool ENABLE_VALIDATION_LAYERS = false;
  // #endif
  if (ENABLE_VALIDATION_LAYERS &&
      !checkValidationLayersSupport(validation_layers)) {
    throw std::runtime_error("Validation layers requested, but not available!");
  }

  // Get required extensions for GLFW.
  uint32_t glfw_extension_count = 0;
  const char** glfw_extensions =
      glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  auto engine = tupi::Engine::create(
      "Hello Triangle", VK_MAKE_VERSION(0, 1, 0),
      tupi::ExtensionSet(glfw_extension_count, glfw_extensions),
      validation_layers);
  auto surface = tupi::glfw::Surface::create(engine, window);
  auto physical_devices = tupi::PhysicalDevice::enumerate(engine);
  std::ranges::sort(physical_devices, std::greater<>{}, [](auto&& x) {
    return x->deviceType() == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
  });
  auto swapchain_support_details =
      physical_devices | tupi::hasExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME) |
      std::views::transform([&surface](auto&& x) {
        return tupi::SwapchainSupportDetail(x, surface);
      }) |
      tupi::hasSurfaceFormat() | tupi::hasPresentMode();
  auto swapchain_support_detail =
      *std::ranges::begin(swapchain_support_details);
  auto physical_device = swapchain_support_detail.physicalDevice();
  auto surface_format = swapchain_support_detail.surfaceFormat();
  std::cout << surface_format.has_value() << std::endl;
  auto present_mode = swapchain_support_detail.presentMode();
  std::cout << present_mode.has_value() << std::endl;
  auto extent = swapchain_support_detail.swapchainExtent(WIDTH, HEIGHT);
  std::cout << extent.width << " " << extent.height << std::endl;
  auto queue_families = tupi::QueueFamily::enumerate(physical_device);
  auto graphics_queue_family =
      *std::ranges::find_if(queue_families, tupi::hasGraphics);
  auto logical_device = tupi::LogicalDevice::create(
      physical_device,
      tupi::LogicalDevice::QueueCreateInfoList{{graphics_queue_family, {1.0f}}},
      tupi::ExtensionSet{VK_KHR_SWAPCHAIN_EXTENSION_NAME});
  auto graphics_queue = tupi::Queue{logical_device, graphics_queue_family, 0};
  auto present_queue_family = graphics_queue_family;
  if (!graphics_queue_family.hasPresentSupport(surface)) {
    present_queue_family =
        *std::ranges::find_if(queue_families, tupi::hasPresentSupport{surface});
  }
  auto present_queue = tupi::Queue{logical_device, present_queue_family, 0};

  auto swapchain =
      tupi::Swapchain::create(logical_device, swapchain_support_detail,
                              graphics_queue_family, present_queue_family);

  auto vertex_shader = tupi::Shader::create(
      logical_device, "../shaders/vert.spv", tupi::Shader::Vertex);
  auto fragment_shader = tupi::Shader::create(
      logical_device, "../shaders/frag.spv", tupi::Shader::Fragment);

  auto vertex_input = tupi::PipelineVertexInput{};
  auto input_assembly = tupi::PipelineInputAssembly{};
  auto viewport_state = tupi::PipelineViewportState{
      VkViewport{0.0f, 0.0f, static_cast<float>(extent.width),
                 static_cast<float>(extent.height), 0.0f, 1.0f},
      VkRect2D{{0, 0}, extent}};
  auto rasterization_state = tupi::PipelineRasterizationState{};
  auto multisample_state = tupi::PipelineMultisampleState{};
  auto color_blend_state = tupi::PipelineColorBlendState(
      std::vector<VkPipelineColorBlendAttachmentState>{
          VkPipelineColorBlendAttachmentState{
              VK_FALSE, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO,
              VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO,
              VK_BLEND_OP_ADD,
              VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                  VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT}});
  auto dynamic_state = tupi::PipelineDynamicState{};
  auto pipeline_layout = tupi::PipelineLayout{logical_device};

  auto attachment_descriptions =
      tupi::AttachmentDescriptionVec{tupi::AttachmentDescription{
          0, swapchain->format(), VK_SAMPLE_COUNT_1_BIT,
          VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE,
          VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
          VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR}};
  auto subpass_description =
      tupi::SubpassDescription(tupi::AttachmentReferenceVec{
          VkAttachmentReference{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}});
  auto subpass_descriptions = tupi::SubpassDescriptionVec{subpass_description};
  auto render_pass = tupi::RenderPass::create(
      logical_device, std::move(attachment_descriptions),
      std::move(subpass_descriptions));
  auto pipeline = tupi::Pipeline::create(
      logical_device, tupi::ShaderPtrVec{vertex_shader, fragment_shader},
      std::move(vertex_input), input_assembly, std::move(viewport_state),
      std::move(rasterization_state), std::move(multisample_state),
      std::move(color_blend_state), std::move(dynamic_state),
      std::move(pipeline_layout), render_pass);
  auto framebuffers = tupi::Framebuffer::enumerate(swapchain, render_pass);
  auto command_pool =
      tupi::CommandPool::create(logical_device, graphics_queue_family);

  constexpr int MAX_FRAMES_IN_FLIGHT = 2;
  tupi::FrameVec frames;
  frames.reserve(MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    frames.emplace_back(logical_device, command_pool);
  }

  uint32_t current_frame = 0;
  while (!glfwWindowShouldClose(window->handle())) {
    glfwPollEvents();
    frames.at(current_frame)
        .draw(swapchain, framebuffers, pipeline, graphics_queue, present_queue);
    current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  logical_device->waitIdle();

  glfwTerminate();

  return 0;
}
