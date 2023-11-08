#pragma once

#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "tupi/fwd.h"
#include "tupi/internal/resource.h"

namespace tupi {
class Engine : public internal::SharedResource<Engine> {
 public:
  Engine(Token, const std::string& app_name, uint32_t app_version,
         const ExtensionSet& extensions,
         const std::vector<const char*>& validation_layers = {});
  ~Engine();

  auto handle() const -> VkInstance;

 protected:
  Engine(const Engine&) = delete;
  Engine(Engine&&) = delete;
  Engine& operator=(const Engine&) = delete;
  Engine& operator=(Engine&&) = delete;

 private:
  VkInstance instance_{VK_NULL_HANDLE};
};

inline auto Engine::handle() const -> VkInstance { return instance_; }
}  // namespace tupi
