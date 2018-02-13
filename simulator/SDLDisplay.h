#include <cstdint>
#include <memory>
#include "SDL.h"

namespace smartwatch {
  namespace simulator {
    class SDLDisplay {
      public:
        enum Color {eBlack,eWhite,eInvert};

        SDLDisplay();
        SDLDisplay(SDLDisplay&) = default;
        ~SDLDisplay() = default;

        void write();
        void wait();
        void draw(std::uint16_t x, std::uint16_t y, Color color);

      private:
        std::shared_ptr<SDL_Window> window_;
        std::shared_ptr<SDL_Renderer> renderer_;
    };
  }
}