#pragma once

namespace tupi {
class IWindow {
 public:
  virtual ~IWindow() = default;

  virtual auto extent() const -> VkExtent2D = 0;
};
}  // namespace tupi
