#ifndef TUPI_GLTF_TEXTURE_H
#define TUPI_GLTF_TEXTURE_H

#include <optional>
#include <string>

namespace tupi::gltf {
struct Texture {
  std::optional<uint32_t> sampler;
  std::optional<uint32_t> source;
  std::optional<std::string> name;
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_TEXTURE_H