#ifndef TUPI_GLFW_INPUT_H
#define TUPI_GLFW_INPUT_H

#include "tupi/frame.h"
#include "tupi/input.h"
#include "tupi/tick.h"

namespace tupi::glfw {
class KeyboardObserver : public IKeyboardObserver {
 public:
  auto onKeyPressed(GLFWwindow* window, int key, int modifiers)
      -> void override;
  auto onKeyReleased(GLFWwindow* window, int key, int modifiers)
      -> void override;
  auto onKeyRepeated(GLFWwindow* window, int key, int modifiers)
      -> void override;
};

class MouseObserver : public IMouseObserver {
 public:
  auto onMouseMove(GLFWwindow* window, double x, double y) -> void override;
  auto onMouseRelease(GLFWwindow* window, int button, int modifiers)
      -> void override;
};

class DefaultCameraInput : public IKeyboardObserver,
                           public IMouseObserver,
                           public ITickObserver {
 public:
  DefaultCameraInput(CameraSharedPtr camera);

  auto onKeyPressed(GLFWwindow* window, int key, int modifiers)
      -> void override;
  auto onKeyReleased(GLFWwindow* window, int key, int modifiers)
      -> void override;
  auto onKeyRepeated(GLFWwindow* window, int key, int modifiers)
      -> void override;
  auto onMouseMove(GLFWwindow* window, double x, double y) -> void override;
  auto onMousePress(GLFWwindow* window, int button, int modifiers)
      -> void override;
  auto onMouseRelease(GLFWwindow* window, int button, int modifiers)
      -> void override;
  auto tick(float seconds) -> void override;

 private:
  enum class KeyMask : uint32_t {
    Forward = 1,
    Left = 1 << 1,
    Backward = 1 << 2,
    Right = 1 << 3
  };

  enum class MouseButtonMask : uint8_t {
    Left = 1,
    Middle = 1 << 1,
    Right = 1 << 2
  };

  CameraSharedPtr camera_;
  uint32_t pressed_keys_{0};
  uint8_t pressed_mouse_buttons_{0};
  float mouse_sensitivity_{0.005f};
  float move_velocity_{1.5f};
  double mouse_press_x_;
  double mouse_press_y_;
};
}  // namespace tupi::glfw
#endif  // TUPI_GLFW_INPUT_H