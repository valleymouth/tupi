#ifndef TUPI_GLTF_MATERIAL_H
#define TUPI_GLTF_MATERIAL_H

#include <optional>
#include <string>

namespace tupi::gltf {
struct Material {
  enum class AlphaMode { Opaque, Mask, Blend };

  struct TextureInfo {
    uint32_t index;
    uint32_t tex_coord{0};
  };

  struct PbrMetallicRoughness {
    glm::vec4 base_color_factor{1.0f, 1.0f, 1.0f, 1.0f};
    std::optional<TextureInfo> base_color_texture;
    float metallic_factor{1.0f};
    float roughness_factor{1.0f};
    std::optional<TextureInfo> metallic_roughness_texture;
  };

  struct OcclusionTexture {
    uint32_t index;
    uint32_t tex_coord{0};
    float strength{1.0f};
  };

  struct NormalTexture {
    uint32_t index;
    uint32_t tex_coord{0};
    float scale{1.0f};
  };

  std::optional<std::string> name;
  std::optional<PbrMetallicRoughness> pbr_metallic_roughness;
  std::optional<NormalTexture> normal_texture;
  std::optional<OcclusionTexture> occlusion_texture;
  std::optional<TextureInfo> emissive_texture;
  glm::vec3 emissive_factor{0.0f, 0.0f, 0.0f};
  AlphaMode alpha_mode{AlphaMode::Opaque};
  float alpha_cutoff{0.5f};
  bool double_sided{false};
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_MATERIAL_H