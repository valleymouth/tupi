#include <algorithm>
#include <chrono>
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "tupi/buffer.h"
#include "tupi/command_buffer.h"
#include "tupi/command_pool.h"
#include "tupi/descriptor_pool.h"
#include "tupi/descriptor_set.h"
#include "tupi/descriptor_set_layout.h"
#include "tupi/engine.h"
#include "tupi/extension_set.h"
#include "tupi/fence.h"
#include "tupi/frame.h"
#include "tupi/framebuffer.h"
#include "tupi/glfw/surface.h"
#include "tupi/glfw/window.h"
#include "tupi/image_2d.h"
#include "tupi/image_view.h"
#include "tupi/logical_device.h"
#include "tupi/physical_device.h"
#include "tupi/pipeline.h"
#include "tupi/pipeline_color_blend_state.h"
#include "tupi/pipeline_depth_stencil_state.h"
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
#include "tupi/sampler.h"
#include "tupi/semaphore.h"
#include "tupi/shader.h"
#include "tupi/subpass_description.h"
#include "tupi/surface.h"
#include "tupi/swapchain.h"
#include "tupi/swapchain_support_detail.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 texcoord;

  static auto bindingDescription() -> tupi::VertexInputBindingDescriptionVec {
    tupi::VertexInputBindingDescriptionVec result{1};
    result[0].binding = 0;
    result[0].stride = sizeof(Vertex);
    result[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return result;
  }

  static auto attributeDescriptions()
      -> tupi::VertexInputAttributeDescriptionVec {
    tupi::VertexInputAttributeDescriptionVec result{3};
    result[0].binding = 0;
    result[0].location = 0;
    result[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    result[0].offset = offsetof(Vertex, position);
    result[1].binding = 0;
    result[1].location = 1;
    result[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    result[1].offset = offsetof(Vertex, color);
    result[2].binding = 0;
    result[2].location = 2;
    result[2].format = VK_FORMAT_R32G32_SFLOAT;
    result[2].offset = offsetof(Vertex, texcoord);
    return result;
  }
};

struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

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
      tupi::hasFeature(tupi::PhysicalDevice::Feature::SamplerAnisotropy) |
      std::views::transform([&surface](auto&& x) {
        return tupi::SwapchainSupportDetail(x, surface);
      }) |
      tupi::hasSurfaceFormat() | tupi::hasPresentMode();
  auto swapchain_support_detail =
      *std::ranges::begin(swapchain_support_details);
  auto physical_device = swapchain_support_detail.physicalDevice();
  auto surface_format = swapchain_support_detail.surfaceFormat();
  auto present_mode = swapchain_support_detail.presentMode();
  auto extent = swapchain_support_detail.swapchainExtent(WIDTH, HEIGHT);
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
      logical_device, "../../../shaders/vert.spv", tupi::Shader::Vertex);
  auto fragment_shader = tupi::Shader::create(
      logical_device, "../../../shaders/frag.spv", tupi::Shader::Fragment);

  auto vertex_input = tupi::PipelineVertexInput::create<Vertex>();
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
  auto depth_stencil_state = tupi::PipelineDepthStencilState{};
  auto dynamic_state = tupi::PipelineDynamicState{};
  auto descriptor_set_layout = tupi::DescriptorSetLayout::create(
      logical_device,
      tupi::DescriptorSetLayoutBindingVec{
          tupi::DescriptorSetLayoutBinding{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                           1, VK_SHADER_STAGE_VERTEX_BIT},
          tupi::DescriptorSetLayoutBinding{
              1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,
              VK_SHADER_STAGE_FRAGMENT_BIT}});
  auto pipeline_layout = tupi::PipelineLayout::create(
      logical_device, tupi::DescriptorSetLayoutPtrVec{descriptor_set_layout});

  auto attachment_descriptions = tupi::AttachmentDescriptionVec{
      {0, swapchain->format(), VK_SAMPLE_COUNT_1_BIT,
       VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE,
       VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
       VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR},
      {0, physical_device->findDepthFormat(), VK_SAMPLE_COUNT_1_BIT,
       VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE,
       VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
       VK_IMAGE_LAYOUT_UNDEFINED,
       VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL}};
  auto subpass_description = tupi::SubpassDescription(
      tupi::AttachmentReferenceVec{
          {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}},
      {1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL});
  auto subpass_descriptions = tupi::SubpassDescriptionVec{subpass_description};
  auto render_pass = tupi::RenderPass::create(
      logical_device, std::move(attachment_descriptions),
      std::move(subpass_descriptions));
  auto pipeline = tupi::Pipeline::create(
      logical_device, tupi::ShaderPtrVec{vertex_shader, fragment_shader},
      std::move(vertex_input), input_assembly, std::move(viewport_state),
      std::move(rasterization_state), std::move(multisample_state),
      std::move(color_blend_state), std::move(depth_stencil_state),
      std::move(dynamic_state), std::move(pipeline_layout), render_pass);
  auto framebuffers = tupi::Framebuffer::enumerate(*swapchain, render_pass);
  auto command_pool =
      tupi::CommandPool::create(logical_device, graphics_queue_family);

  const std::vector<Vertex> vertices = {
      {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
      {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
      {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
      {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
      {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
      {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
      {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
      {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};
  const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};

  auto vertex_buffer = tupi::Buffer::create<Vertex>(
      logical_device, static_cast<uint32_t>(vertices.size()),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  tupi::Buffer::copy<Vertex>(vertices, vertex_buffer, command_pool,
                             graphics_queue);

  auto index_buffer = tupi::Buffer::create<uint16_t>(
      logical_device, static_cast<uint32_t>(indices.size()),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  tupi::Buffer::copy<uint16_t>(indices, index_buffer, command_pool,
                               graphics_queue);

  auto image = tupi::Image2D::create(command_pool, graphics_queue,
                                     "../../../textures/statue.png");
  auto image_view =
      tupi::ImageView::create(logical_device, image, VK_FORMAT_R8G8B8A8_SRGB);
  auto sampler = tupi::Sampler::create(logical_device);

  constexpr int MAX_FRAMES_IN_FLIGHT = 2;
  tupi::FrameVec frames;
  frames.reserve(MAX_FRAMES_IN_FLIGHT);
  tupi::BufferPtrVec uniform_buffers;
  uniform_buffers.reserve(MAX_FRAMES_IN_FLIGHT);
  auto descriptor_pool = tupi::DescriptorPool::create(
      logical_device,
      tupi::DescriptorPoolSizeVec{
          {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
           static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)},
          {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
           static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)}},
      static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT));
  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    frames.emplace_back(logical_device, command_pool);
    uniform_buffers.emplace_back(tupi::Buffer::create<UniformBufferObject>(
        logical_device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
  }
  auto descriptor_sets = tupi::DescriptorSet::create(
      descriptor_pool, {descriptor_set_layout, descriptor_set_layout},
      uniform_buffers, {image_view, image_view}, {sampler, sampler});

  // tupi::gltf::Reader reader;
  // reader.read("../models/WaterBottle/WaterBottle.gltf");

  uint32_t current_frame = 0;
  while (!glfwWindowShouldClose(window->handle())) {
    glfwPollEvents();

    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(
                     currentTime - startTime)
                     .count();
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
                            glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view =
        glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(
        glm::radians(45.0f), extent.width / (float)extent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;
    uniform_buffers.at(current_frame)->copy(ubo, true);  // keep it mapped

    frames.at(current_frame)
        .draw(swapchain, framebuffers, pipeline, graphics_queue, present_queue,
              {descriptor_sets.at(current_frame)},
              tupi::BufferPtrVec{vertex_buffer}, tupi::OffsetVec{0},
              static_cast<uint32_t>(vertices.size()), index_buffer, 0,
              static_cast<uint32_t>(indices.size()));
    current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  logical_device->waitIdle();

  glfwTerminate();

  return 0;
}
