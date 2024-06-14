#include "tupi/gltf/mesh_primitive.h"

#include <sstream>

#include "tupi/gltf/accessor.h"
#include "tupi/gltf/file.h"

namespace tupi::gltf {
auto forEach(MeshPrimitive& mesh_primitive, const std::string& attribute,
             File& file, std::function<void(const glm::vec4&, std::uint64_t)> f)
    -> void {
  auto iterator = mesh_primitive.attributes.find(attribute);
  if (iterator != mesh_primitive.attributes.end()) {
    file.accessors.at(iterator->second).forEach(file, f);
  }
}

auto MeshPrimitive::forEachPosition(
    File& file, std::function<void(const glm::vec4&, std::uint64_t)> f)
    -> void {
  forEach(*this, "POSITION", file, f);
}

auto MeshPrimitive::forEachNormal(
    File& file, std::function<void(const glm::vec4&, std::uint64_t)> f)
    -> void {
  forEach(*this, "NORMAL", file, f);
}

auto MeshPrimitive::forEachTextureCoordinate(
    File& file, uint32_t index,
    std::function<void(const glm::vec4&, std::uint64_t)> f) -> void {
  std::stringstream ss;
  ss << "TEXCOORD_" << index;
  forEach(*this, ss.str(), file, f);
}

auto MeshPrimitive::forEachTangent(
    File& file, std::function<void(const glm::vec4&, std::uint64_t)> f)
    -> void {
  forEach(*this, "TANGENT", file, f);
}

auto MeshPrimitive::forEachIndex(File& file,
                                 std::function<void(uint32_t, std::uint64_t)> f)
    -> void {
  if (indices.has_value()) {
    file.accessors.at(indices.value()).forEach(file, f);
  }
}

}  // namespace tupi::gltf