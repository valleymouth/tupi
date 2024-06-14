#ifndef TUPI_GLTF_BUFFER_VIEW_H
#define TUPI_GLTF_BUFFER_VIEW_H

#include <functional>
#include <string>

#include "tupi/gltf/buffer.h"
#include "tupi/gltf/file.h"

namespace tupi::gltf {
struct BufferView {
  template <typename F>
  auto forEach(File& file, Accessor& accessor, F f) -> void {
    file.buffers.at(buffer).forEach(file, accessor, *this, f);
  }

  uint32_t buffer;
  uint64_t byte_offset{0};
  uint64_t byte_length;
  uint8_t byte_stride{0};
  std::string name;
  // Missing target, extensions, and extras.
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_BUFFER_VIEW_H