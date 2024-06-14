#include "tupi/gltf/buffer.h"

#include <fstream>

#include "tupi/gltf/accessor.h"
#include "tupi/gltf/buffer_view.h"
#include "tupi/gltf/file.h"

namespace tupi::gltf {
auto Buffer::load(const std::filesystem::path& path) -> bool {
  if (!isLoaded() && byte_length > 0) {
    std::filesystem::path uri_path = path.parent_path() / uri;
    if (std::filesystem::exists(uri_path)) {
      std::ifstream input(uri_path.string(), std::ios::binary);
      buffer.resize(byte_length);
      input.read(reinterpret_cast<char*>(buffer.data()), byte_length);
      return true;
    }
  }
  return false;
}

auto Buffer::forEach(File& file, Accessor& accessor, BufferView& buffer_view,
                     std::function<void(const glm::vec4&, std::uint64_t)> f)
    -> void {
  if (!isLoaded()) {
    if (!load(file.path)) {
      return;
    }
  }
  if (accessor.component_type == Accessor::ComponentType::Float) {
    auto* current =
        buffer.data() + accessor.byte_offset + buffer_view.byte_offset;
    uint64_t count = 0;
    while (count < accessor.count) {
      auto* data = reinterpret_cast<float*>(current);
      glm::vec4 vec{0, 0, 0, 1};
      for (uint8_t i = 0; i < Accessor::size(accessor.type); i++) {
        vec[i] = data[i];
      }
      f(vec, count++);
      current += buffer_view.byte_stride;
    }
  }
}

auto Buffer::forEach(File& file, Accessor& accessor, BufferView& buffer_view,
                     std::function<void(uint32_t, std::uint64_t)> f) -> void {
  if (!isLoaded()) {
    if (!load(file.path)) {
      return;
    }
  }
  if (accessor.component_type == Accessor::ComponentType::UnsignedShort) {
    auto* current =
        buffer.data() + accessor.byte_offset + buffer_view.byte_offset;
    uint64_t count = 0;
    while (count < accessor.count) {
      auto* data = reinterpret_cast<uint16_t*>(current);
      f(static_cast<uint32_t>(*data), count++);
      if (buffer_view.byte_stride == 0) {  // Tightly packed.
        current += sizeof(uint16_t);
      } else {
        current += buffer_view.byte_stride;
      }
    }
  }
}

}  // namespace tupi::gltf