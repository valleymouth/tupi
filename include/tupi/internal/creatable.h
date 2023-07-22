#pragma once

#include <memory>

namespace tupi::internal {
template <typename T, template <typename> typename P = std::unique_ptr>
class Creatable {
 public:
  using Pointer = P<T>;

  template <typename... Ts>
  static auto create(Ts&&... args) -> Pointer {
    auto result = P<T>(new T(std::forward<Ts>(args)...));
    // TODO: check usefulness.
    // result->postInitialize();
    return result;
  }

  // protected:
  //  virtual auto postInitialize() -> void {}
};
}  // namespace tupi::internal
