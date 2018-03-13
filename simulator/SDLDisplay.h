#include <cstdint>
#include <memory>
#include "Display.h"
#include "SDL.h"

namespace smartwatch {
  namespace simulator {
    class SDLDisplay final : public smartwatch::Display {
      public:
        SDLDisplay(std::uint8_t scale = 1);
        SDLDisplay(SDLDisplay&) = default;
        ~SDLDisplay() = default;

        void write() override;
        void wait() override;
        void draw(std::uint16_t x, std::uint16_t y, Display::Color color) override;

      private:
        std::uint8_t scale_;
        std::shared_ptr<SDL_Window> window_;
        std::shared_ptr<SDL_Renderer> renderer_;
    };
  }
}