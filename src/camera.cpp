#include "tupi/camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace tupi {
Camera::Camera(const CameraInfo& camera_info)
    : near_(camera_info.near),
      far_(camera_info.far),
      fov_(camera_info.fov),
      aspect_ratio_(camera_info.aspect_ratio) {
  projection_ = glm::perspective(fov_, aspect_ratio_, near_, far_);
  // Vulkan has y pointing down and glm::perspective is made for OpenGL which
  // has y pointing up. This flips y to fix it.
  projection_[1][1] *= -1;
}
}  // namespace tupi