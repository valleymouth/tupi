#pragma once

#include "tupi/command_pool.h"
#include "tupi/handle.h"
#include "tupi/logical_device.h"

namespace tupi {
class ScopedSingleCommand {
 public:
 protected:
  ScopedSingleCommand(LogicalDevicePtr logical_device,
                      CommandPoolPtr command_pool);

 private:
  LogicalDevicePtr logical_device_{};
  CommandPoolPtr command_pool_{};
  Handle<VkCommandBuffer> command_buffer_{};
};
}  // namespace tupi