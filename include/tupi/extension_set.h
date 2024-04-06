#pragma once

#include <initializer_list>
#include <string>
#include <vector>

#include "tupi/fwd.h"

namespace tupi {
class ExtensionSet {
  using Container = std::vector<std::string>;

 public:
  using Iterator = typename Container::iterator;
  using ConstIterator = typename Container::const_iterator;

  ExtensionSet() = default;
  ExtensionSet(uint32_t count, const char** extensions);
  ExtensionSet(std::initializer_list<const char*> extensions);

  auto addExtension(std::string name) -> void;
  auto hasExtension(const std::string& name) const -> bool;
  auto size() const -> std::size_t;
  auto begin() -> Iterator;
  auto begin() const -> ConstIterator;
  auto end() -> Iterator;
  auto end() const -> ConstIterator;
  auto toCStringVector() const -> std::vector<const char*>;

  static auto enumerate() -> ExtensionSet;
  static auto enumerate(const PhysicalDeviceSharedPtr& physical_device)
      -> ExtensionSet;

 private:
  Container extension_names_;
};
}  // namespace tupi
