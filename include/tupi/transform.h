#ifndef TUPI_TRANSFORM_H
#define TUPI_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tupi {
struct Transform {
  glm::vec3 forward() const { return rotation * glm::vec3{1.0f, 0.0f, 0.0f}; }
  glm::vec3 left() const { return rotation * glm::vec3{0.0f, 1.0f, 0.0f}; }
  glm::vec3 up() const { return rotation * glm::vec3{0.0f, 0.0f, 1.0f}; }
  glm::vec3 backward() const { return -forward(); }
  glm::vec3 right() const { return -left(); }
  glm::vec3 down() const { return -up(); }
  glm::mat4 matrix() const {
    auto result = glm::mat4_cast(rotation);
    result[3] = glm::vec4(position, 1.0f);
    result[0][0] *= scale.x;
    result[1][1] *= scale.y;
    result[2][2] *= scale.z;
    return result;
  }

  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::quat rotation = glm::quat(1.0f, {0.0f, 0.0f, 0.0f});
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
};
}  // namespace tupi

#endif  // TUPI_TRANSFORM_H