#ifndef TUPI_GLTF_BUFFER_H
#define TUPI_GLTF_BUFFER_H

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "tupi/fwd.h"

namespace tupi::gltf {
struct Buffer {
  auto isLoaded() const -> bool { return !buffer.empty(); }

  auto load(const std::filesystem::path& path) -> bool;

  auto unload() -> bool {
    if (isLoaded()) {
      buffer.clear();
      return true;
    }
    return false;
  }

  auto forEach(File& file, Accessor& accessor, BufferView& buffer_view,
               std::function<void(const glm::vec4&, std::uint64_t)> f) -> void;
  auto forEach(File& file, Accessor& accessor, BufferView& buffer_view,
               std::function<void(uint32_t, std::uint64_t)> f) -> void;

  std::filesystem::path uri;
  uint64_t byte_length;
  std::string name;
  // Missing extension, and extras.
  std::vector<std::byte> buffer;
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_BUFFER_H