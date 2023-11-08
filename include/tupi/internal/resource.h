#pragma once

#include <memory>

namespace tupi::internal {
template <typename T>
class UniqueResource {
 protected:
  class Token {};

 public:
  using Pointer = std::unique_ptr<T>;

  template <typename... Ts>
  static auto create(Ts&&... args) -> Pointer {
    return std::make_unique<T>(Token{}, std::forward<Ts>(args)...);
  }
};

template <typename T>
class SharedResource {
 protected:
  class Token {};

 public:
  using Pointer = std::shared_ptr<T>;

  template <typename... Ts>
  static auto create(Ts&&... args) -> Pointer {
    return std::make_shared<T>(Token{}, std::forward<Ts>(args)...);
  }
};
}  // namespace tupi::internal
