#include "tupi/gltf/file.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "tupi/gltf/accessor.h"
#include "tupi/gltf/buffer.h"
#include "tupi/gltf/buffer_view.h"
#include "tupi/gltf/image.h"
#include "tupi/gltf/material.h"
#include "tupi/gltf/mesh.h"
#include "tupi/gltf/mesh_primitive.h"
#include "tupi/gltf/node.h"
#include "tupi/gltf/sampler.h"
#include "tupi/gltf/scene.h"
#include "tupi/gltf/texture.h"

namespace tupi::gltf {

auto readTextureInfo(const nlohmann::json& json) -> Material::TextureInfo {
  Material::TextureInfo texture_info;
  for (const auto& [key, value] : json.items()) {
    if (key == "index") {
      texture_info.index = value.get<uint32_t>();
    } else if (key == "texCoord") {
      texture_info.tex_coord = value.get<uint32_t>();
    }
  }
  return texture_info;
}

auto readNormalTexture(const nlohmann::json& json) -> Material::NormalTexture {
  Material::NormalTexture normal_texture;
  for (const auto& [key, value] : json.items()) {
    if (key == "index") {
      normal_texture.index = value.get<uint32_t>();
    } else if (key == "texCoord") {
      normal_texture.tex_coord = value.get<uint32_t>();
    } else if (key == "scale") {
      normal_texture.scale = value.get<float>();
    }
  }
  return normal_texture;
}

auto readOcclusionTexture(const nlohmann::json& json)
    -> Material::OcclusionTexture {
  Material::OcclusionTexture occlusion_texture;
  for (const auto& [key, value] : json.items()) {
    if (key == "index") {
      occlusion_texture.index = value.get<uint32_t>();
    } else if (key == "texCoord") {
      occlusion_texture.tex_coord = value.get<uint32_t>();
    } else if (key == "strength") {
      occlusion_texture.strength = value.get<float>();
    }
  }
  return occlusion_texture;
}

template <typename T>
auto readVector(const nlohmann::json& json) -> T {
  T result;
  int index = 0;
  for (const auto& elem : json) {
    result[index++] = elem.get<float>();
  }
  return result;
}

auto File::load(const std::filesystem::path& path) -> void {
  if (!std::filesystem::exists(path)) return;
  this->path = path;
  std::ifstream gltf_file(path.string());
  auto json = nlohmann::json::parse(gltf_file);
  if (json["asset"]["version"] != "2.0") {
    throw std::runtime_error(
        "Unsuported glTF file version: expected 2.0, got " +
        json["asset"]["version"]);
  }
  uint32_t default_scene = json["scene"].get<int32_t>();
  for (const auto& json_scene : json["scenes"]) {
    Scene scene;
    for (const auto& node : json_scene["nodes"]) {
      scene.nodes.push_back(node.get<uint32_t>());
    }
    scenes.push_back(std::move(scene));
  }
  for (const auto& json_node : json["nodes"]) {
    Node node;
    for (const auto& [key, value] : json_node.items()) {
      if (key == "name") {
        node.name = value.get<std::string>();
      } else if (key == "mesh") {
        node.mesh = value.get<uint32_t>();
      }
    }
    nodes.push_back(std::move(node));
  }
  for (const auto& json_mesh : json["meshes"]) {
    Mesh mesh;
    for (const auto& [mesh_key, mesh_value] : json_mesh.items()) {
      if (mesh_key == "name") {
        mesh.name = mesh_value;
      } else if (mesh_key == "primitives") {
        MeshPrimitive primitive;
        for (const auto& prim : mesh_value) {
          for (const auto& [prim_key, prim_value] : prim.items()) {
            if (prim_key == "attributes") {
              for (const auto& [attr_key, attr_value] : prim_value.items()) {
                primitive.attributes[attr_key] = attr_value;
              }
            } else if (prim_key == "indices") {
              primitive.indices = prim_value.get<uint32_t>();
            } else if (prim_key == "material") {
              primitive.material = prim_value.get<uint32_t>();
            }
          }
        }
        mesh.primitives.push_back(std::move(primitive));
      }
    }
    meshes.push_back(std::move(mesh));
  }
  for (const auto& json_accessor : json["accessors"]) {
    Accessor accessor;
    for (const auto& [accessor_key, accessor_value] : json_accessor.items()) {
      if (accessor_key == "bufferView") {
        accessor.buffer_view = accessor_value.get<uint32_t>();
      } else if (accessor_key == "byteOffset") {
        accessor.byte_offset = accessor_value.get<uint32_t>();
      } else if (accessor_key == "componentType") {
        accessor.component_type = accessor_value.get<Accessor::ComponentType>();
      } else if (accessor_key == "normalized") {
        accessor.normalized = accessor_value.get<bool>();
      } else if (accessor_key == "count") {
        accessor.count = accessor_value.get<uint32_t>();
      } else if (accessor_key == "type") {
        auto type = accessor_value.get<std::string>();
        if (type == "SCALAR") {
          accessor.type = Accessor::Type::Scalar;
        } else if (type == "VEC2") {
          accessor.type = Accessor::Type::Vec2;
        } else if (type == "VEC3") {
          accessor.type = Accessor::Type::Vec3;
        } else if (type == "VEC4") {
          accessor.type = Accessor::Type::Vec4;
        } else if (type == "MAT2") {
          accessor.type = Accessor::Type::Mat2;
        } else if (type == "MAT3") {
          accessor.type = Accessor::Type::Mat3;
        } else if (type == "MAT4") {
          accessor.type = Accessor::Type::Mat4;
        }
      } else if (accessor_key == "name") {
        accessor.name = accessor_value.get<std::string>();
      }
    }
    accessors.push_back(std::move(accessor));
  }
  for (const auto& json_buffer_view : json["bufferViews"]) {
    BufferView buffer_view;
    for (const auto& [buffer_view_key, buffer_view_value] :
         json_buffer_view.items()) {
      if (buffer_view_key == "buffer") {
        buffer_view.buffer = buffer_view_value.get<uint32_t>();
      } else if (buffer_view_key == "byteOffset") {
        buffer_view.byte_offset = buffer_view_value.get<uint64_t>();
      } else if (buffer_view_key == "byteLength") {
        buffer_view.byte_length = buffer_view_value.get<uint64_t>();
      } else if (buffer_view_key == "byteStride") {
        buffer_view.byte_stride = buffer_view_value.get<uint8_t>();
      } else if (buffer_view_key == "name") {
        buffer_view.name = buffer_view_value.get<std::string>();
      }
    }
    buffer_views.push_back(std::move(buffer_view));
  }
  for (const auto& json_buffer : json["buffers"]) {
    Buffer buffer;
    for (const auto& [buffer_key, buffer_value] : json_buffer.items()) {
      if (buffer_key == "uri") {
        buffer.uri = buffer_value.get<std::string>();
      } else if (buffer_key == "byteLength") {
        buffer.byte_length = buffer_value.get<uint64_t>();
      } else if (buffer_key == "name") {
        buffer.name = buffer_value.get<std::string>();
      }
    }
    buffers.push_back(std::move(buffer));
  }
  for (const auto& json_material : json["materials"]) {
    Material material;
    for (const auto& [material_key, material_value] : json_material.items()) {
      if (material_key == "name") {
        material.name = material_value.get<std::string>();
      } else if (material_key == "pbrMetallicRoughness") {
        Material::PbrMetallicRoughness pbr_metallic_roughness;
        for (const auto& [pbr_key, pbr_value] : material_value.items()) {
          if (pbr_key == "baseColorFactor") {
            pbr_metallic_roughness.base_color_factor =
                readVector<glm::vec4>(pbr_value);
          } else if (pbr_key == "baseColorTexture") {
            pbr_metallic_roughness.base_color_texture =
                readTextureInfo(pbr_value);
          } else if (pbr_key == "metallicFactor") {
            pbr_metallic_roughness.metallic_factor = pbr_value.get<float>();
          } else if (pbr_key == "roughnessFactor") {
            pbr_metallic_roughness.roughness_factor = pbr_value.get<float>();
          } else if (pbr_key == "metallicRoughnessTexture") {
            pbr_metallic_roughness.metallic_roughness_texture =
                readTextureInfo(pbr_value);
          }
        }
        material.pbr_metallic_roughness = std::move(pbr_metallic_roughness);
      } else if (material_key == "normalTexture") {
        material.normal_texture = readNormalTexture(material_value);
      } else if (material_key == "occlusionTexture") {
        material.occlusion_texture = readOcclusionTexture(material_value);
      } else if (material_key == "emissiveTexture") {
        material.emissive_texture = readTextureInfo(material_value);
      } else if (material_key == "emissiveFactor") {
        material.emissive_factor = readVector<glm::vec3>(material_value);
      } else if (material_key == "alphaMode") {
        if (material_value.get<std::string>() == "OPAQUE") {
          material.alpha_mode = Material::AlphaMode::Opaque;
        } else if (material_value.get<std::string>() == "MASK") {
          material.alpha_mode = Material::AlphaMode::Mask;
        } else if (material_value.get<std::string>() == "BLEND") {
          material.alpha_mode = Material::AlphaMode::Blend;
        }
      } else if (material_key == "alphaCutoff") {
        material.alpha_cutoff = material_value.get<float>();
      } else if (material_key == "doubleSided") {
        material.double_sided = material_value.get<bool>();
      }
    }
    materials.push_back(std::move(material));
  }
  for (const auto& json_texture : json["textures"]) {
    Texture texture;
    for (const auto& [texture_key, texture_value] : json_texture.items()) {
      if (texture_key == "sampler") {
        texture.sampler = texture_value.get<uint32_t>();
      } else if (texture_key == "source") {
        texture.source = texture_value.get<uint32_t>();
      } else if (texture_key == "name") {
        texture.name = texture_value.get<std::string>();
      }
    }
    textures.push_back(std::move(texture));
  }
  for (const auto& json_image : json["images"]) {
    Image image;
    for (const auto& [image_key, image_value] : json_image.items()) {
      if (image_key == "uri") {
        image.uri = image_value.get<std::string>();
      } else if (image_key == "mimeType") {
        image.mime_type = image_value.get<std::string>();
      } else if (image_key == "bufferView") {
        image.buffer_view = image_value.get<uint32_t>();
      } else if (image_key == "name") {
        image.name = image_value.get<std::string>();
      }
    }
    images.push_back(std::move(image));
  }
  for (const auto& json_sampler : json["samplers"]) {
    Sampler sampler;
    for (const auto& [sampler_key, sampler_value] : json_sampler.items()) {
      if (sampler_key == "magFilter") {
        sampler.mag_filter = sampler_value.get<Sampler::MagFilter>();
      } else if (sampler_key == "minFilter") {
        sampler.min_filter = sampler_value.get<Sampler::MinFilter>();
      } else if (sampler_key == "wrapS") {
        sampler.wrap_s = sampler_value.get<Sampler::Wrap>();
      } else if (sampler_key == "wrapT") {
        sampler.wrap_t = sampler_value.get<Sampler::Wrap>();
      }
    }
    samplers.push_back(std::move(sampler));
  }
}
}  // namespace tupi::gltf