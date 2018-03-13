#include <cstdint>

#ifndef SMARTCHWATCH_DISPLAY_H
#define SMARTCHWATCH_DISPLAY_H

namespace smartwatch {
  class Display {
  public:
    enum class Color {eBlack,eWhite,eInvert};

    virtual void write() = 0;
    virtual void wait() = 0;
    virtual void draw(std::uint16_t x, std::uint16_t y, Color color) = 0;

  protected:
    Display() = default;
  };
}

#endif