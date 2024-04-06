#include "tupi/extension_set.h"

#include <vulkan/vulkan.h>

#include <ranges>

#include "tupi/physical_device.h"

namespace tupi {
ExtensionSet::ExtensionSet(uint32_t count, const char** extensions) {
  for (uint32_t i = 0; i < count; i++) {
    extension_names_.emplace_back(extensions[i]);
  }
  std::ranges::sort(extension_names_);
}

ExtensionSet::ExtensionSet(std::initializer_list<const char*> extensions) {
  for (const auto& extension : extensions) {
    extension_names_.emplace_back(extension);
  }
  std::ranges::sort(extension_names_);
}

auto ExtensionSet::addExtension(std::string name) -> void {
  extension_names_.push_back(std::move(name));
  std::ranges::sort(extension_names_);
}

auto ExtensionSet::hasExtension(const std::string& name) const -> bool {
  return std::ranges::binary_search(extension_names_, name);
}

auto ExtensionSet::size() const -> std::size_t {
  return extension_names_.size();
}

auto ExtensionSet::begin() -> Iterator { return extension_names_.begin(); }

auto ExtensionSet::begin() const -> ConstIterator {
  return extension_names_.begin();
}

auto ExtensionSet::end() -> Iterator { return extension_names_.end(); }

auto ExtensionSet::end() const -> ConstIterator {
  return extension_names_.end();
}

auto ExtensionSet::toCStringVector() const -> std::vector<const char*> {
  std::vector<const char*> result(extension_names_.size());
  std::ranges::transform(extension_names_, result.begin(),
                         [](const auto& x) { return x.c_str(); });
  return result;
}

auto ExtensionSet::enumerate() -> ExtensionSet {
  ExtensionSet result;
  uint32_t count = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
  std::vector<VkExtensionProperties> extensions(count);
  vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());
  for (const auto& extension : extensions) {
    result.extension_names_.emplace_back(extension.extensionName);
  }
  std::ranges::sort(result.extension_names_);
  return result;
}

auto ExtensionSet::enumerate(const PhysicalDeviceSharedPtr& physical_device)
    -> ExtensionSet {
  ExtensionSet result;
  uint32_t count;
  vkEnumerateDeviceExtensionProperties(physical_device->handle(), nullptr,
                                       &count, nullptr);
  std::vector<VkExtensionProperties> extensions(count);
  vkEnumerateDeviceExtensionProperties(physical_device->handle(), nullptr,
                                       &count, extensions.data());
  for (const auto& extension : extensions) {
    result.extension_names_.emplace_back(extension.extensionName);
  }
  std::ranges::sort(result.extension_names_);
  return result;
}
}  // namespace tupi
