#ifndef TUPI_GLTF_ACCESSOR_H
#define TUPI_GLTF_ACCESSOR_H

#include <functional>
#include <optional>
#include <string>

#include "tupi/gltf/buffer_view.h"
#include "tupi/gltf/file.h"

namespace tupi::gltf {
struct Accessor {
  enum class ComponentType : uint16_t {
    None = 0,
    Byte = 5120,
    UnsignedByte = 5121,
    Short = 5122,
    UnsignedShort = 5123,
    UnsignedInt = 5125,
    Float = 5126
  };

  enum class Type : uint8_t { Scalar, Vec2, Vec3, Vec4, Mat2, Mat3, Mat4 };

  static constexpr auto size(Type type) -> uint8_t {
    constexpr uint8_t result[] = {1, 2, 3, 4, 4, 9, 16};
    return result[static_cast<uint8_t>(type)];
  }

  template <typename F>
  auto forEach(File& file, F f) -> void {
    if (buffer_view.has_value()) {
      file.buffer_views.at(buffer_view.value()).forEach(file, *this, f);
    }
  }

  std::optional<uint32_t> buffer_view;
  uint32_t byte_offset{0};
  ComponentType component_type{ComponentType::None};
  bool normalized = false;
  uint32_t count{0};
  Type type{Type::Scalar};
  std::optional<std::string> name;
  // Missing max, min, sparse, extensions, and extras.
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_ACCESSOR_H