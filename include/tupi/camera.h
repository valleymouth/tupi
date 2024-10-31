#ifndef TUPI_CAMERA_H
#define TUPI_CAMERA_H

#include <glm/glm.hpp>

#include "transform.h"

namespace tupi {
struct CameraInfo {
  float near{0.1f};
  float far{1000.0f};
  float fov{60.0f};
  float aspect_ratio{1.0f};
};

class Camera : public Transform {
 public:
  Camera(const CameraInfo& camera_info);

  glm::mat4 getViewMatrix() const {
    return glm::lookAt(position, position + forward(), up());
  }
  glm::mat4 getProjectionMatrix() const { return projection_; }

 private:
  float near_;
  float far_;
  float fov_;
  float aspect_ratio_;
  glm::mat4 projection_;
};
}  // namespace tupi

#endif  // TUPI_CAMERA_H