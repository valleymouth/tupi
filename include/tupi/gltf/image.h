#ifndef TUPI_GLTF_IMAGE_H
#define TUPI_GLTF_IMAGE_H

#include <filesystem>
#include <optional>

namespace tupi::gltf {
struct Image {
  std::optional<std::filesystem::path> uri;
  std::optional<std::string> mime_type;
  std::optional<uint32_t> buffer_view;
  std::optional<std::string> name;
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_IMAGE_H