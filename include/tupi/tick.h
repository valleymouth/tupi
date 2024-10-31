#ifndef TUPI_TICK_H
#define TUPI_TICK_H

namespace tupi {
class ITickObserver {
 public:
  virtual auto tick(float seconds) -> void = 0;
};
}  // namespace tupi

#endif  // TUPI_TICK_H