#include "tupi/texture_2d.h"

namespace tupi {
Texture2D::Texture2D(ImageView2DSharedPtr image_view, SamplerSharedPtr sampler)
    : image_view_(std::move(image_view)), sampler_(std::move(sampler)) {}
}  // namespace tupi