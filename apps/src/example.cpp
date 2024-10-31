#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <variant>

#include "tupi/buffer.h"
#include "tupi/camera.h"
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
#include "tupi/glfw/input.h"
#include "tupi/glfw/surface.h"
#include "tupi/glfw/window.h"
#include "tupi/gltf/accessor.h"
#include "tupi/gltf/buffer.h"
#include "tupi/gltf/buffer_view.h"
#include "tupi/gltf/file.h"
#include "tupi/gltf/image.h"
#include "tupi/gltf/material.h"
#include "tupi/gltf/mesh.h"
#include "tupi/gltf/mesh_primitive.h"
#include "tupi/gltf/node.h"
#include "tupi/gltf/sampler.h"
#include "tupi/gltf/scene.h"
#include "tupi/gltf/texture.h"
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
#include "tupi/texture.h"
#include "tupi/texture_2d.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texcoord;
  glm::vec4 tangent;
  uint32_t material;

  static auto bindingDescription() -> tupi::VertexInputBindingDescriptionVec {
    tupi::VertexInputBindingDescriptionVec result{1};
    result[0].binding = 0;
    result[0].stride = sizeof(Vertex);
    result[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return result;
  }

  static auto attributeDescriptions()
      -> tupi::VertexInputAttributeDescriptionVec {
    tupi::VertexInputAttributeDescriptionVec result{5};
    result[0].binding = 0;
    result[0].location = 0;
    result[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    result[0].offset = offsetof(Vertex, position);
    result[1].binding = 0;
    result[1].location = 1;
    result[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    result[1].offset = offsetof(Vertex, normal);
    result[2].binding = 0;
    result[2].location = 2;
    result[2].format = VK_FORMAT_R32G32_SFLOAT;
    result[2].offset = offsetof(Vertex, texcoord);
    result[3].binding = 0;
    result[3].location = 3;
    result[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    result[3].offset = offsetof(Vertex, tangent);
    result[4].binding = 0;
    result[4].location = 4;
    result[4].format = VK_FORMAT_R32_UINT;
    result[4].offset = offsetof(Vertex, material);
    return result;
  }
};

struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  // For transforming normals from object space to world space, see
  // https://en.wikipedia.org/wiki/Normal_(geometry)#Transforming_normals.
  alignas(16) glm::mat4 model_inverse_transpose;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 projection;
  alignas(16) glm::vec3 eye;
};

struct MaterialBufferObject {
  static constexpr uint32_t invalid_texture_index = (1 << 16) - 1;

  alignas(16) glm::uvec4 textures{invalid_texture_index, invalid_texture_index,
                                  invalid_texture_index, invalid_texture_index};
  alignas(16) glm::vec4 base_color_factor{1.0f, 1.0f, 1.0f, 1.0f};
  alignas(16) glm::vec4 metallic_roughness_occlusion_factor{1.0f, 1.0f, 1.0f,
                                                            0.0f};
};

int main() {
  constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
  constexpr uint32_t MAX_BINDLESS_RESOURCES = 1024;
  constexpr uint32_t MAX_DESCRIPTOR_SETS_PER_FRAME = 32;
  constexpr uint32_t MAX_MATERIAL_COUNT = 256;

  glfwInit();

  constexpr uint32_t WIDTH = 800;
  constexpr uint32_t HEIGHT = 600;
  auto window = std::make_shared<tupi::glfw::Window>(WIDTH, HEIGHT);

  // Need to ensure null terminated strings.
  const std::vector<const char*> validation_layers = {
      "VK_LAYER_KHRONOS_validation"};

  // Get required extensions for GLFW.
  uint32_t glfw_extension_count = 0;
  const char** glfw_extensions =
      glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  auto engine = std::make_shared<tupi::Engine>(
      "Hello Triangle", VK_MAKE_VERSION(0, 1, 0),
      tupi::ExtensionSet(glfw_extension_count, glfw_extensions),
      validation_layers);
  auto surface = std::make_shared<tupi::glfw::Surface>(engine, window);
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
  auto logical_device = std::make_shared<tupi::LogicalDevice>(
      physical_device,
      tupi::QueueCreateInfoVec{{graphics_queue_family, {1.0f}}},
      tupi::ExtensionSet{VK_KHR_SWAPCHAIN_EXTENSION_NAME});
  auto graphics_queue = tupi::Queue{logical_device, graphics_queue_family, 0};
  auto present_queue_family = graphics_queue_family;
  if (!graphics_queue_family.hasPresentSupport(surface)) {
    present_queue_family =
        *std::ranges::find_if(queue_families, tupi::hasPresentSupport{surface});
  }
  auto present_queue = tupi::Queue{logical_device, present_queue_family, 0};

  auto swapchain = std::make_shared<tupi::Swapchain>(
      logical_device, swapchain_support_detail, graphics_queue_family,
      present_queue_family);

  auto vertex_shader = std::make_shared<tupi::Shader>(
      logical_device, "../../../shaders/vert.spv", tupi::Shader::Vertex);
  auto fragment_shader = std::make_shared<tupi::Shader>(
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
  auto descriptor_set_layout =
      tupi::DescriptorSetLayout::create<tupi::DescriptorSetLayout>(
          logical_device,
          tupi::DescriptorSetLayoutBindingVec{
              tupi::DescriptorSetLayoutBinding{
                  0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
                  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},
              tupi::DescriptorSetLayoutBinding{
                  1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
                  VK_SHADER_STAGE_FRAGMENT_BIT}});
  auto bindless_descriptor_set_layout =
      tupi::DescriptorSetLayout::create<tupi::BindlessDescriptorSetLayout>(
          logical_device,
          tupi::DescriptorSetLayoutBindingVec{tupi::DescriptorSetLayoutBinding{
              0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
              MAX_BINDLESS_RESOURCES, VK_SHADER_STAGE_FRAGMENT_BIT}});
  auto pipeline_layout = std::make_shared<tupi::PipelineLayout>(
      logical_device,
      tupi::DescriptorSetLayoutSharedPtrVec{descriptor_set_layout,
                                            bindless_descriptor_set_layout});

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
  auto render_pass = std::make_shared<tupi::RenderPass>(
      logical_device, std::move(attachment_descriptions),
      std::move(subpass_descriptions));
  auto pipeline = std::make_shared<tupi::Pipeline>(
      logical_device, tupi::ShaderSharedPtrVec{vertex_shader, fragment_shader},
      std::move(vertex_input), input_assembly, std::move(viewport_state),
      std::move(rasterization_state), std::move(multisample_state),
      std::move(color_blend_state), std::move(depth_stencil_state),
      std::move(dynamic_state), std::move(pipeline_layout), render_pass);
  auto framebuffers = tupi::Framebuffer::enumerate(swapchain, render_pass);
  auto command_pool = std::make_shared<tupi::CommandPool>(
      logical_device, graphics_queue_family);

  tupi::gltf::File gltf_file;
  gltf_file.load("../../../resources/FlightHelmet.gltf");

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  for (auto& mesh : gltf_file.meshes) {
    for (auto& primitive : mesh.primitives) {
      uint32_t vertex_offset = vertices.size();
      primitive.forEachPosition(
          gltf_file, [&](const glm::vec4& position, uint64_t) {
            Vertex vertex{};
            // Converting glTF Z forward, X left and Y up
            // coordinates.
            vertex.position = glm::vec3(position.z, position.x, position.y);
            vertex.material = primitive.material.value();
            vertices.push_back(vertex);
          });
      primitive.forEachNormal(gltf_file,
                              [&](const glm::vec4& normal, uint64_t index) {
                                // Converting glTF Z forward, X left and Y up
                                // coordinates.
                                vertices[index + vertex_offset].normal =
                                    glm::vec3(normal.z, normal.x, normal.y);
                              });
      primitive.forEachTextureCoordinate(
          gltf_file, 0, [&](const glm::vec4& texcoord, uint64_t index) {
            vertices[index + vertex_offset].texcoord = glm::vec2(texcoord);
          });
      primitive.forEachTangent(
          gltf_file, [&](const glm::vec4& tangent, uint64_t index) {
            // Converting glTF Z forward, X left and Y up
            // coordinates.
            vertices[index + vertex_offset].tangent =
                glm::vec4(tangent.z, tangent.x, tangent.y, tangent.w);
          });
      std::uint32_t max_index = 0;
      primitive.forEachIndex(gltf_file, [&](uint32_t vertex_index, uint64_t) {
        indices.push_back(vertex_index + vertex_offset);
        max_index = std::max(max_index, vertex_index);
      });
    }
  }

  auto vertex_buffer = tupi::Buffer::createShared<Vertex>(
      logical_device, static_cast<uint32_t>(vertices.size()),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  tupi::Buffer::copy<Vertex>(vertices, vertex_buffer, command_pool,
                             graphics_queue);

  auto index_buffer = tupi::Buffer::createShared<uint32_t>(
      logical_device, static_cast<uint32_t>(indices.size()),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  tupi::Buffer::copy<uint32_t>(indices, index_buffer, command_pool,
                               graphics_queue);

  std::vector<MaterialBufferObject> materials;
  std::vector<tupi::IImageSharedPtr> images(gltf_file.images.size());

  auto createImageForTexture = [&](uint32_t texture_index, VkFormat format) {
    auto gltf_texture = gltf_file.textures.at(texture_index);
    if (gltf_texture.source.has_value()) {
      auto image_index = gltf_texture.source.value();
      if (!images.at(image_index)) {
        auto gltf_image = gltf_file.images.at(image_index);
        if (gltf_image.uri.has_value()) {
          auto uri = gltf_file.path.parent_path() / gltf_image.uri.value();
          images.at(image_index) = tupi::Image2D::createFromFile(
              command_pool, graphics_queue,
              std::filesystem::absolute(uri).string(), format);
        }
      }
    }
  };

  for (const auto& gltf_material : gltf_file.materials) {
    MaterialBufferObject material{};
    if (gltf_material.pbr_metallic_roughness.has_value()) {
      tupi::gltf::Material::PbrMetallicRoughness pbr_metallic_roughness =
          gltf_material.pbr_metallic_roughness.value();
      if (pbr_metallic_roughness.base_color_texture.has_value()) {
        tupi::gltf::Material::TextureInfo texture_info =
            pbr_metallic_roughness.base_color_texture.value();
        material.textures.x = texture_info.index;
        // TODO: check if tex_coord is 0
        createImageForTexture(texture_info.index, VK_FORMAT_R8G8B8A8_SRGB);
      }
      if (pbr_metallic_roughness.metallic_roughness_texture.has_value()) {
        tupi::gltf::Material::TextureInfo texture_info =
            pbr_metallic_roughness.metallic_roughness_texture.value();
        material.textures.y = texture_info.index;
        // TODO: check if tex_coord is 0
        createImageForTexture(texture_info.index, VK_FORMAT_R8G8B8A8_UNORM);
      }
      material.base_color_factor = pbr_metallic_roughness.base_color_factor;
      material.metallic_roughness_occlusion_factor.x =
          pbr_metallic_roughness.metallic_factor;
      material.metallic_roughness_occlusion_factor.y =
          pbr_metallic_roughness.roughness_factor;
    }
    if (gltf_material.normal_texture.has_value()) {
      tupi::gltf::Material::NormalTexture normal_texture =
          gltf_material.normal_texture.value();
      material.textures.z = normal_texture.index;
      // TODO: check if tex_coord is 0
      createImageForTexture(normal_texture.index, VK_FORMAT_R8G8B8A8_UNORM);
    }
    if (gltf_material.occlusion_texture.has_value()) {
      tupi::gltf::Material::OcclusionTexture occlusion_texture =
          gltf_material.occlusion_texture.value();
      material.textures.w = occlusion_texture.index;
      // TODO: check if tex_coord is 0
      material.metallic_roughness_occlusion_factor.z =
          occlusion_texture.strength;
      createImageForTexture(occlusion_texture.index, VK_FORMAT_R8G8B8A8_UNORM);
    }
    materials.push_back(std::move(material));
  }

  std::vector<tupi::SamplerSharedPtr> samplers;
  for (const auto& gltf_sampler : gltf_file.samplers) {
    VkFilter mag_filter{VK_FILTER_LINEAR};
    if (gltf_sampler.mag_filter == tupi::gltf::Sampler::MagFilter::Nearest) {
      mag_filter = VK_FILTER_NEAREST;
    }
    VkFilter min_filter{VK_FILTER_LINEAR};
    if (gltf_sampler.min_filter == tupi::gltf::Sampler::MinFilter::Nearest) {
      min_filter = VK_FILTER_NEAREST;
    }
    VkSamplerAddressMode address_mode_u{VK_SAMPLER_ADDRESS_MODE_REPEAT};
    if (gltf_sampler.wrap_s == tupi::gltf::Sampler::Wrap::Repeat) {
      address_mode_u = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    } else if (gltf_sampler.wrap_s ==
               tupi::gltf::Sampler::Wrap::MirroredRepeat) {
      address_mode_u = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    } else if (gltf_sampler.wrap_s == tupi::gltf::Sampler::Wrap::ClampToEdge) {
      address_mode_u = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }
    VkSamplerAddressMode address_mode_v{VK_SAMPLER_ADDRESS_MODE_REPEAT};
    if (gltf_sampler.wrap_s == tupi::gltf::Sampler::Wrap::Repeat) {
      address_mode_v = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    } else if (gltf_sampler.wrap_s ==
               tupi::gltf::Sampler::Wrap::MirroredRepeat) {
      address_mode_v = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    } else if (gltf_sampler.wrap_s == tupi::gltf::Sampler::Wrap::ClampToEdge) {
      address_mode_v = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }
    samplers.emplace_back(
        std::make_shared<tupi::Sampler>(logical_device, mag_filter, min_filter,
                                        address_mode_u, address_mode_v));
  }

  std::vector<tupi::ITextureSharedPtr> textures;
  for (const auto& gltf_texture : gltf_file.textures) {
    const auto& image = images.at(gltf_texture.source.value());
    auto image_view = std::make_shared<tupi::ImageView2D>(logical_device, image,
                                                          image->format());
    auto texture = std::make_shared<tupi::Texture2D>(
        image_view, samplers.at(gltf_texture.sampler.value()));
    textures.push_back(std::move(texture));
  }

  tupi::FrameVec frames;
  frames.reserve(MAX_FRAMES_IN_FLIGHT);
  tupi::BufferSharedPtrVec uniform_buffers;
  uniform_buffers.reserve(MAX_FRAMES_IN_FLIGHT);
  tupi::BufferSharedPtrVec material_buffers;
  material_buffers.reserve(MAX_FRAMES_IN_FLIGHT);
  auto descriptor_pool = std::make_shared<tupi::DescriptorPool>(
      logical_device,
      tupi::DescriptorPoolSizeVec{
          {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT},
          {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, MAX_FRAMES_IN_FLIGHT},
          {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_BINDLESS_RESOURCES}},
      MAX_FRAMES_IN_FLIGHT * MAX_DESCRIPTOR_SETS_PER_FRAME);
  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    frames.emplace_back(logical_device, command_pool);
    uniform_buffers.emplace_back(
        tupi::Buffer::createShared<UniformBufferObject>(
            logical_device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
    material_buffers.emplace_back(
        tupi::Buffer::createShared<MaterialBufferObject>(
            logical_device, static_cast<uint32_t>(materials.size()),
            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
    tupi::Buffer::copy<MaterialBufferObject>(materials, material_buffers.back(),
                                             command_pool, graphics_queue);
  }
  auto descriptor_sets = tupi::DescriptorSet::create(
      descriptor_pool, {descriptor_set_layout, descriptor_set_layout,
                        bindless_descriptor_set_layout});
  tupi::WriteDescriptorSetVec writes = {
      {descriptor_sets.at(0), uniform_buffers.at(0)},
      {descriptor_sets.at(0), material_buffers.at(0), 1},
      {descriptor_sets.at(1), uniform_buffers.at(1)},
      {descriptor_sets.at(1), material_buffers.at(1), 1}};
  uint32_t array_element = 0;
  for (const auto& texture : textures) {
    writes.push_back({descriptor_sets.at(2), texture, 0, array_element++});
  }
  tupi::DescriptorSet::update(logical_device, writes);

  tupi::CameraSharedPtr camera = std::make_shared<tupi::Camera>(
      tupi::CameraInfo{0.1f, 10.0f, glm::radians(45.0f),
                       extent.width / (float)extent.height});
  camera->position = glm::vec3{1.0f, 0.0f, 1.0f};
  camera->rotation =
      glm::quat({0.0f, glm::radians(30.0f), glm::radians(180.0f)});

  tupi::glfw::DefaultCameraInputSharedPtr camera_input =
      std::make_shared<tupi::glfw::DefaultCameraInput>(camera);
  window->addKeyboardObserver(camera_input);
  window->addMouseObserver(camera_input);
  for (auto& frame : frames) {
    frame.addTickObserver(camera_input);
  }

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
    ubo.model_inverse_transpose = glm::inverseTranspose(ubo.model);
    ubo.view = camera->getViewMatrix();
    ubo.projection = camera->getProjectionMatrix();
    ubo.eye = camera->position;
    uniform_buffers.at(current_frame)->copy(ubo, true);  // keep it mapped

    frames.at(current_frame)
        .draw(swapchain, framebuffers, pipeline, graphics_queue, present_queue,
              {descriptor_sets.at(current_frame), descriptor_sets.at(2)},
              tupi::BufferSharedPtrVec{vertex_buffer}, tupi::OffsetVec{0},
              static_cast<uint32_t>(vertices.size()), index_buffer, 0,
              static_cast<uint32_t>(indices.size()));
    current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  logical_device->waitIdle();

  glfwTerminate();

  return 0;
}
