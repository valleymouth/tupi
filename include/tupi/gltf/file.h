#ifndef TUPI_GLTF_FILE_H
#define TUPI_GLTF_FILE_H

#include <filesystem>
#include <functional>
#include <vector>

#include "tupi/fwd.h"

namespace tupi::gltf {
struct File {
  using NodeTraverseFunction =
      std::function<bool(const Node& node, const glm::mat4& matrix)>;

  auto load(const std::filesystem::path& path) -> void;
  auto traverseNodes(uint32_t scene_index, NodeTraverseFunction function)
      -> void;

  uint32_t default_scene{};
  std::filesystem::path path;
  std::vector<Scene> scenes;
  std::vector<Node> nodes;
  std::vector<Mesh> meshes;
  std::vector<Accessor> accessors;
  std::vector<BufferView> buffer_views;
  std::vector<Buffer> buffers;
  std::vector<Material> materials;
  std::vector<Texture> textures;
  std::vector<Image> images;
  std::vector<Sampler> samplers;
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_FILE_H