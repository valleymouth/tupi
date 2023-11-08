#include "tupi/internal/resource.h"

#include <vector>

#include "tupi/utility.h"

class Test : public tupi::internal::UniqueResource<Test> {
 private:
  using Base = tupi::internal::UniqueResource<Test>;

 public:
  Test(Base::Token, int value) : value(value) {}
  auto handle() const -> int { return value; }
  int value;
};

auto main() -> int {
  auto test = Test::create(5);
  if (test->value != 5) return 1;
  if (test->handle() != 5) return 1;
  std::vector<Test::Pointer> tests;
  tests.emplace_back(Test::create(2));
  tests.emplace_back(Test::create(4));
  tests.emplace_back(Test::create(8));
  tests.emplace_back(Test::create(16));
  if (tupi::handles(tests)[0] != 2) return 1;
  if (tupi::handles(tests)[1] != 4) return 1;
  if (tupi::handles(tests)[2] != 8) return 1;
  if (tupi::handles(tests)[3] != 16) return 1;

  return 0;
}