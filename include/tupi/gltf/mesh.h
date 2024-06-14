#ifndef TUPI_GLTF_MESH_H
#define TUPI_GLTF_MESH_H

#include <optional>
#include <string>
#include <vector>

#include "tupi/fwd.h"

namespace tupi::gltf {
struct Mesh {
  std::optional<std::string> name;
  std::vector<MeshPrimitive> primitives;
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_MESH_H