#include <cstdint>
#include <memory>
#include "SDL.h"
#include "Display.h"
#include "SDLDisplay.h"

constexpr auto SCREEN_WIDTH = 200;
constexpr auto SCREEN_HEIGHT = SCREEN_WIDTH;

namespace smartwatch {
  namespace simulator {
    SDLDisplay::SDLDisplay(std::uint8_t scale) : scale_(scale), smartwatch::Display() {
      window_ = std::shared_ptr<SDL_Window>(SDL_CreateWindow("Smartwatch",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH * scale_,
        SCREEN_HEIGHT * scale_,
        SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI), SDL_DestroyWindow);
      renderer_ = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);
    }

    void SDLDisplay::wait() {
      // Simulate waiting for a write to complete.
      SDL_Delay(500 /*ms*/);
      SDL_PumpEvents();
    }

    void SDLDisplay::write() {
      SDL_RenderPresent(renderer_.get());
    }

    void SDLDisplay::draw(std::uint16_t x, std::uint16_t y, Color color) {
      const std::uint8_t rgb_value = (color == Color::eWhite ? 255 : 0);
      SDL_SetRenderDrawColor(renderer_.get(), rgb_value, rgb_value, rgb_value, SDL_ALPHA_OPAQUE);

      const SDL_Rect pixel = { x * scale_, y * scale_, scale_, scale_ };
      SDL_RenderFillRect(renderer_.get(), &pixel);
    }
  }
}