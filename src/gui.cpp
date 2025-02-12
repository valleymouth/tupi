#include "tupi/gui.h"

#include "third_party/imgui/imgui.h"
#include "third_party/imgui/imgui_impl_glfw.h"
#include "third_party/imgui/imgui_impl_vulkan.h"
#include "tupi/descriptor_pool.h"
#include "tupi/engine.h"
#include "tupi/glfw/window.h"
#include "tupi/logical_device.h"
#include "tupi/physical_device.h"
#include "tupi/queue.h"
#include "tupi/render_pass.h"

namespace tupi {
Gui::Gui(const glfw::Window& window, RenderPassSharedPtr render_pass,
         const Queue& graphics_queue, const QueueFamily& graphics_queue_family,
         uint32_t max_frames_in_flight)
    : descriptor_pool_(
          render_pass->logicalDevice(),
          DescriptorPoolSizeVec{{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1}},
          1),
      render_pass_(std::move(render_pass)) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  auto logical_device = render_pass_->logicalDevice();
  auto physical_device = logical_device->physicalDevice();

  ImGui_ImplGlfw_InitForVulkan(window.handle(), true);
  ImGui_ImplVulkan_InitInfo imgui_init_info{};
  imgui_init_info.Instance = physical_device->engine()->handle();
  imgui_init_info.PhysicalDevice = physical_device->handle();
  imgui_init_info.Device = logical_device->handle();
  imgui_init_info.QueueFamily = graphics_queue_family.index();
  imgui_init_info.Queue = graphics_queue.handle();
  imgui_init_info.DescriptorPool = descriptor_pool_.handle();
  imgui_init_info.RenderPass = render_pass_->handle();
  imgui_init_info.Subpass = 0;
  imgui_init_info.MinImageCount = 2;
  imgui_init_info.ImageCount = max_frames_in_flight;
  imgui_init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  ImGui_ImplVulkan_Init(&imgui_init_info);
}

Gui::~Gui() {
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

auto Gui::render() -> void {
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow(&show_demo_window_);
  ImGui::Render();
}

auto Gui::makeCommand() -> Command {
  return tupi::Command{10, [](tupi::CommandBuffer& command_buffer) {
                         ImDrawData* draw_data = ImGui::GetDrawData();
                         ImGui_ImplVulkan_RenderDrawData(
                             draw_data, command_buffer.handle());
                       }};
}
}  // namespace tupi