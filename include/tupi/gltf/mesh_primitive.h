#ifndef TUPI_GLTF_MESH_PRIMITIVE_H
#define TUPI_GLTF_MESH_PRIMITIVE_H

#include <functional>
#include <map>
#include <optional>
#include <string>

#include "tupi/fwd.h"

namespace tupi::gltf {
struct MeshPrimitive {
  enum class Mode {
    Points,
    Lines,
    LineLoop,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan
  };

  Mode mode{Mode::Triangles};
  std::optional<uint32_t> indices;
  // Map of attribute semantic to accessor index.
  // Possible attribute semantics: POSITION, NORMAL, TANGENT,
  // TEXCOORD_n, COLOR_n, JOINTS_n and WEIGHTS_n
  std::map<std::string, uint32_t> attributes;
  std::optional<uint32_t> material;

  auto forEachPosition(File& file,
                       std::function<void(const glm::vec4&, std::uint64_t)> f)
      -> void;
  auto forEachNormal(File& file,
                     std::function<void(const glm::vec4&, std::uint64_t)> f)
      -> void;
  auto forEachTextureCoordinate(
      File& file, uint32_t index,
      std::function<void(const glm::vec4&, std::uint64_t)> f) -> void;
  auto forEachTangent(File& file,
                      std::function<void(const glm::vec4&, std::uint64_t)> f)
      -> void;
  auto forEachIndex(File& file, std::function<void(uint32_t, std::uint64_t)> f)
      -> void;
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_MESH_PRIMITIVE_H