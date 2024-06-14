#ifndef TUPI_GLTF_SCENE_H
#define TUPI_GLTF_SCENE_H

#include <vector>

namespace tupi::gltf {
struct Scene {
  std::vector<uint32_t> nodes;
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_SCENE_H