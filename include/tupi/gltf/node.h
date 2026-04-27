#ifndef TUPI_GLTF_NODE_H
#define TUPI_GLTF_NODE_H

#include <optional>
#include <string>
#include <vector>

namespace tupi::gltf {
struct Node {
  uint32_t index;
  std::optional<std::string> name;
  std::optional<uint32_t> mesh;
  std::optional<uint32_t> camera;
  std::optional<glm::mat4> matrix;
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::quat rotation = glm::quat(1.0f, {0.0f, 0.0f, 0.0f});
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
  std::vector<uint32_t> children;
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_NODE_H