#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/handle.h"

namespace tupi {
class Engine {
 public:
  Engine(const std::string& app_name, uint32_t app_version,
         const ExtensionSet& extensions,
         const std::vector<const char*>& validation_layers = {});
  ~Engine();
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  Engine(Engine&& other) = default;
  Engine& operator=(Engine&& other) = default;

  auto handle() const -> VkInstance { return instance_.handle; }

  static bool checkValidationLayersSupport(
      const std::vector<const char*>& validation_layers);

 private:
  Handle<VkInstance> instance_{};
};
}  // namespace tupi
