#pragma once

namespace tupi::internal {
class NonCopyable {
 protected:
  NonCopyable() = default;
  ~NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  auto operator=(const NonCopyable&) -> NonCopyable& = delete;
};
}  // namespace tupi::internal