#pragma once

#include <memory>
#include <ranges>

namespace tupi::internal {
template <typename T, template <typename> typename P>
class Resource {
 public:
  using Pointer = P<T>;

  template <typename... Ts>
  static auto create(Ts&&... args) -> Pointer {
    auto result = P<T>(new T(std::forward<Ts>(args)...));
    return result;
  }

  template <typename R>
  static auto handles(const R& range) {
    return range | std::views::transform([](auto&& x) { return x->handle(); });
  }
};

template <typename T>
using UniqueResource = Resource<T, std::unique_ptr>;

template <typename T>
using SharedResource = Resource<T, std::shared_ptr>;
}  // namespace tupi::internal
