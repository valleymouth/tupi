#ifndef TUPI_GUI_H
#define TUPI_GUI_H

#include "tupi/command_buffer.h"
#include "tupi/descriptor_pool.h"

namespace tupi {
class Gui {
 public:
  Gui(const glfw::Window& window, RenderPassSharedPtr render_pass,
      const Queue& graphics_queue, const QueueFamily& graphics_queue_family,
      uint32_t max_frames_in_flight);
  ~Gui();

  auto render() -> void;
  auto showDemoWindow(bool value) { show_demo_window_ = value; }
  auto makeCommand() -> Command;

 private:
  DescriptorPool descriptor_pool_;
  RenderPassSharedPtr render_pass_{};
  bool show_demo_window_{false};
};
}  // namespace tupi
#endif  // TUPI_GUI_H