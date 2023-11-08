#pragma once

#include <ranges>
#include <vector>

namespace tupi {
template <typename R>
inline auto handles(R&& range) {
  using T = decltype((*std::ranges::begin(range))->handle());
  std::vector<T> result(std::ranges::size(range));
  typename std::vector<T>::size_type index = 0;
  for (auto&& x : range) {
    result[index++] = x->handle();
  }
  return result;
}
}  // namespace tupi