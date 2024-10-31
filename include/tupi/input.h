#ifndef TUPI_INPUT_H
#define TUPI_INPUT_H

#include "tupi/fwd.h"

namespace tupi {
class IKeyboardObserver {
 public:
  virtual auto onKeyPressed(GLFWwindow* window, int key, int modifiers)
      -> void {}
  virtual auto onKeyReleased(GLFWwindow* window, int key, int modifiers)
      -> void {}
  virtual auto onKeyRepeated(GLFWwindow* window, int key, int modifiers)
      -> void {}
};

class IMouseObserver {
 public:
  virtual auto onMouseMove(GLFWwindow* window, double x, double y) -> void {}
  virtual auto onMousePress(GLFWwindow* window, int button, int modifiers)
      -> void {}
  virtual auto onMouseRelease(GLFWwindow* window, int button, int modifiers)
      -> void {}
  virtual auto onMouseScroll(GLFWwindow* window, double x, double y) -> void {}
};
}  // namespace tupi
#endif  // TUPI_INPUT_H