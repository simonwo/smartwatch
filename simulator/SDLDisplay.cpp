#include <cstdint>
#include <memory>
#include "SDL.h"
#include "SDLDisplay.h"

namespace smartwatch {
  namespace simulator {
    SDLDisplay::SDLDisplay() {
      window_ = std::shared_ptr<SDL_Window>(SDL_CreateWindow("Smartwatch",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        200,
        200,
        SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI), SDL_DestroyWindow);
      renderer_ = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);
    }

    void SDLDisplay::wait() {
      // Simulate waiting for a write to complete.
      SDL_Delay(500 /*ms*/);
    }

    void SDLDisplay::write() {
      SDL_RenderPresent(renderer_.get());
    }

    void SDLDisplay::draw(std::uint16_t x, std::uint16_t y, Color color) {
      const std::uint8_t rgb_value = (color == SDLDisplay::eWhite ? 255 : 0);
      SDL_SetRenderDrawColor(renderer_.get(), rgb_value, rgb_value, rgb_value, SDL_ALPHA_OPAQUE);
      SDL_RenderDrawPoint(renderer_.get(), x, y);
    }
  }
}