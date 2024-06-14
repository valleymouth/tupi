#ifndef TUPI_GLTF_SAMPLER_H
#define TUPI_GLTF_SAMPLER_H

#include <optional>

namespace tupi::gltf {
struct Sampler {
  enum class MagFilter : uint16_t { Nearest = 9728, Linear = 9729 };
  enum class MinFilter : uint16_t {
    Nearest = 9728,
    Linear = 9729,
    NearestMipmapNearest = 9984,
    LinearMipmapNearest = 9985,
    NearestMipmapLinear = 9986,
    LinearMipmapLinear = 9987
  };
  enum class Wrap : uint16_t {
    ClampToEdge = 33071,
    MirroredRepeat = 33648,
    Repeat = 10497
  };

  std::optional<MagFilter> mag_filter;
  std::optional<MinFilter> min_filter;
  Wrap wrap_s{Wrap::Repeat};
  Wrap wrap_t{Wrap::Repeat};
};
}  // namespace tupi::gltf
#endif  // TUPI_GLTF_SAMPLER_H