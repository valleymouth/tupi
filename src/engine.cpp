#include "tupi/engine.h"

#include "tupi/extension_set.h"

namespace tupi {
Engine::Engine(const std::string& app_name, uint32_t app_version,
               const ExtensionSet& extensions,
               const std::vector<const char*>& validation_layers) {
#ifdef DEBUG
  constexpr bool ENABLE_VALIDATION_LAYERS = true;
#else
  constexpr bool ENABLE_VALIDATION_LAYERS = false;
#endif
  if (ENABLE_VALIDATION_LAYERS &&
      !checkValidationLayersSupport(validation_layers)) {
    throw std::runtime_error("Validation layers requested, but not available!");
  }

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
  if (ENABLE_VALIDATION_LAYERS && !validation_layers.empty()) {
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

bool Engine::checkValidationLayersSupport(
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
}  // namespace tupi
