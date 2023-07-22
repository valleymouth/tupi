#include "tupi/surface.h"

#include "tupi/engine.h"

namespace tupi {
Surface::~Surface() {
  vkDestroySurfaceKHR(engine_->handle(), handle(), nullptr);
}

Surface::Surface(EnginePtr engine, WindowPtr window)
    : engine_(std::move(engine)), window_(std::move(window)) {}
}  // namespace tupi
