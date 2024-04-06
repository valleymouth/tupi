#ifndef TUPI_HANDLE_H
#define TUPI_HANDLE_H

#include <vulkan/vulkan.hpp>

namespace tupi {
template <typename T>
struct Handle {
  T handle{VK_NULL_HANDLE};

  Handle() = default;
  Handle(T handle) : handle(handle) {}
  ~Handle() = default;
  Handle(const Handle&) = default;
  Handle& operator=(const Handle&) = default;
  Handle(Handle&& other) { *this = std::move(other); }
  Handle& operator=(Handle&& other) {
    handle = other.handle;
    other.handle = VK_NULL_HANDLE;
    return *this;
  }
  // Is this a good idea? I think so. But I am not sure.
  operator T() const { return handle; }
  auto operator&() -> T* { return &handle; }
  auto operator&() const -> const T* { return &handle; }
};
}  // namespace tupi

#endif  // TUPI_HANDLE_H
