#include "tupi/glfw/input.h"

#include <iostream>

#include "tupi/camera.h"
#include "tupi/frame.h"
#include "tupi/glfw/window.h"

namespace tupi::glfw {
auto KeyboardObserver::onKeyPressed(GLFWwindow* window, int key, int modifiers)
    -> void {
  std::cout << "Key pressed -> key: " << key << ", modifier: " << modifiers
            << std::endl;
}
auto KeyboardObserver::onKeyReleased(GLFWwindow* window, int key, int modifiers)
    -> void {
  std::cout << "Key released -> key: " << key << ", modifier: " << modifiers
            << std::endl;
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}
auto KeyboardObserver::onKeyRepeated(GLFWwindow* window, int key, int modifiers)
    -> void {
  std::cout << "Key repeated -> key: " << key << ", modifier: " << modifiers
            << std::endl;
}

auto MouseObserver::onMouseMove(GLFWwindow* window, double x, double y)
    -> void {
  std::cout << "Mouse move -> x: " << x << ", y: " << y << std::endl;
}

auto MouseObserver::onMouseRelease(GLFWwindow* window, int button,
                                   int modifiers) -> void {
  std::cout << "Mouse release -> button: " << button
            << ", modifiers: " << modifiers << std::endl;
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

DefaultCameraInput::DefaultCameraInput(CameraSharedPtr camera)
    : camera_(std::move(camera)) {}

auto DefaultCameraInput::onKeyPressed(GLFWwindow* window, int key,
                                      int modifiers) -> void {
  std::cout << "Key pressed -> key: " << key << ", modifier: " << modifiers
            << std::endl;
  // WASD keys.
  if (key == GLFW_KEY_W) {
    pressed_keys_ |= static_cast<uint32_t>(KeyMask::Forward);
  } else if (key == GLFW_KEY_A) {
    pressed_keys_ |= static_cast<uint32_t>(KeyMask::Left);
  } else if (key == GLFW_KEY_S) {
    pressed_keys_ |= static_cast<uint32_t>(KeyMask::Backward);
  } else if (key == GLFW_KEY_D) {
    pressed_keys_ |= static_cast<uint32_t>(KeyMask::Right);
  }
}
auto DefaultCameraInput::onKeyReleased(GLFWwindow* window, int key,
                                       int modifiers) -> void {
  std::cout << "Key released -> key: " << key << ", modifier: " << modifiers
            << std::endl;
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
  // WASD keys.
  if (key == GLFW_KEY_W) {
    pressed_keys_ &= ~static_cast<uint32_t>(KeyMask::Forward);
  } else if (key == GLFW_KEY_A) {
    pressed_keys_ &= ~static_cast<uint32_t>(KeyMask::Left);
  } else if (key == GLFW_KEY_S) {
    pressed_keys_ &= ~static_cast<uint32_t>(KeyMask::Backward);
  } else if (key == GLFW_KEY_D) {
    pressed_keys_ &= ~static_cast<uint32_t>(KeyMask::Right);
  }
}
auto DefaultCameraInput::onKeyRepeated(GLFWwindow* window, int key,
                                       int modifiers) -> void {
  std::cout << "Key repeated -> key: " << key << ", modifier: " << modifiers
            << std::endl;
}

auto DefaultCameraInput::onMouseMove(GLFWwindow* window, double x, double y)
    -> void {
  std::cout << "Mouse move -> x: " << x << ", y: " << y << std::endl;
  if (pressed_mouse_buttons_ & static_cast<uint8_t>(MouseButtonMask::Right)) {
    camera_->rotation = glm::angleAxis(static_cast<float>(y - mouse_press_y_) *
                                           mouse_sensitivity_,
                                       camera_->left()) *
                        glm::angleAxis(static_cast<float>(x - mouse_press_x_) *
                                           mouse_sensitivity_,
                                       glm::vec3{0.0f, 0.0f, -1.0f}) *
                        camera_->rotation;
    mouse_press_x_ = x;
    mouse_press_y_ = y;
  }
}

auto DefaultCameraInput::onMousePress(GLFWwindow* window, int button,
                                      int modifiers) -> void {
  std::cout << "Mouse press -> button: " << button
            << ", modifiers: " << modifiers << std::endl;
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    pressed_mouse_buttons_ |= static_cast<uint8_t>(MouseButtonMask::Left);
  } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
    pressed_mouse_buttons_ |= static_cast<uint8_t>(MouseButtonMask::Middle);
  } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    pressed_mouse_buttons_ |= static_cast<uint8_t>(MouseButtonMask::Right);
    glfwGetCursorPos(window, &mouse_press_x_, &mouse_press_y_);
  }
}

auto DefaultCameraInput::onMouseRelease(GLFWwindow* window, int button,
                                        int modifiers) -> void {
  std::cout << "Mouse release -> button: " << button
            << ", modifiers: " << modifiers << std::endl;
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    pressed_mouse_buttons_ &= ~static_cast<uint8_t>(MouseButtonMask::Left);
  } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
    pressed_mouse_buttons_ &= ~static_cast<uint8_t>(MouseButtonMask::Middle);
  } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    pressed_mouse_buttons_ &= ~static_cast<uint8_t>(MouseButtonMask::Right);
  }
}

auto DefaultCameraInput::tick(float seconds) -> void {
  std::cout << "Delta time: " << seconds << std::endl;

  if (pressed_mouse_buttons_ & static_cast<uint8_t>(MouseButtonMask::Right)) {
    // WASD keys.
    if (pressed_keys_ & static_cast<uint32_t>(KeyMask::Forward)) {
      camera_->position += camera_->forward() * seconds * move_velocity_;
    } else if (pressed_keys_ & static_cast<uint32_t>(KeyMask::Left)) {
      camera_->position += camera_->left() * seconds * move_velocity_;
    } else if (pressed_keys_ & static_cast<uint32_t>(KeyMask::Backward)) {
      camera_->position += camera_->backward() * seconds * move_velocity_;
    } else if (pressed_keys_ & static_cast<uint32_t>(KeyMask::Right)) {
      camera_->position += camera_->right() * seconds * move_velocity_;
    }
    // Mouse look.
  }
}
}  // namespace tupi::glfw