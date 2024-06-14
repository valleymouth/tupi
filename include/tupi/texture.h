#ifndef TUPI_TEXTURE_H
#define TUPI_TEXTURE_H

#include "tupi/fwd.h"

namespace tupi {
class ITexture {
 public:
  virtual auto imageView() const -> IImageViewSharedPtr = 0;
  virtual auto sampler() const -> SamplerSharedPtr = 0;
};
}  // namespace tupi
#endif  // TUPI_TEXTURE_H