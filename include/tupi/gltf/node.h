#ifndef TUPI_GLTF_NODE_H
#define TUPI_GLTF_NODE_H

#include <optional>
#include <string>
#include <vector>

namespace tupi::gltf {
struct Node {
  std::optional<std::string> name;
  std::optional<uint32_t> mesh;
  std::optional<uint32_t> camera;
  std::vector<Node> children;
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_NODE_H