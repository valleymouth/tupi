#ifndef TUPI_TEXTURE_2D_H
#define TUPI_TEXTURE_2D_H

#include "tupi/fwd.h"
#include "tupi/image_view_2d.h"
#include "tupi/texture.h"

namespace tupi {
class Texture2D : public ITexture {
 public:
  Texture2D(ImageView2DSharedPtr image_view, SamplerSharedPtr sampler);

  auto imageView() const -> IImageViewSharedPtr override { return image_view_; }
  auto sampler() const -> SamplerSharedPtr override { return sampler_; }

 private:
  ImageView2DSharedPtr image_view_;
  SamplerSharedPtr sampler_;
};
}  // namespace tupi
#endif  // TUPI_TEXTURE_2D_H