#ifndef TUPI_WINDOW_H
#define TUPI_WINDOW_H

#include "tupi/fwd.h"

namespace tupi {
class IWindow {
 public:
  virtual ~IWindow() = default;

  virtual auto extent() const -> VkExtent2D = 0;

  virtual auto addKeyboardObserver(IKeyboardObserverSharedPtr keyboard_observer)
      -> void = 0;
  virtual auto addMouseObserver(IMouseObserverSharedPtr mouse_observer)
      -> void = 0;
};
}  // namespace tupi
#endif  // TUPI_WINDOW_H