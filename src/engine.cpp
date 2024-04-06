#include "tupi/engine.h"

#include "tupi/extension_set.h"

namespace tupi {
Engine::Engine(const std::string& app_name, uint32_t app_version,
               const ExtensionSet& extensions,
               const std::vector<const char*>& validation_layers) {
  VkApplicationInfo vk_app_info{VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                nullptr,
                                app_name.c_str(),
                                app_version,
                                "Tupi",
                                VK_MAKE_API_VERSION(0, 0, 1, 0),
                                VK_API_VERSION_1_0};

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &vk_app_info;
  create_info.enabledExtensionCount = extensions.size();
  std::vector<const char*> extension_names;
  for (auto& extension : extensions) {
    extension_names.emplace_back(extension.data());
  }
  create_info.ppEnabledExtensionNames = extension_names.data();
  if (!validation_layers.empty()) {
    create_info.enabledLayerCount =
        static_cast<uint32_t>(validation_layers.size());
    create_info.ppEnabledLayerNames = validation_layers.data();
  } else {
    create_info.enabledLayerCount = 0;
  }

  if (vkCreateInstance(&create_info, nullptr, &instance_.handle) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create Vulkan instance!");
  }
}

Engine::~Engine() { vkDestroyInstance(instance_.handle, nullptr); }
}  // namespace tupi
