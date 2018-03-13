#include "Platform.h"
#include "Display.h"
#include "ptr.h"

extern const Image picard;

namespace smartwatch {
  Platform::Platform(ptr<Display> lcd)
  : _lcd(lcd) {
  }

  void Platform::init() {
  }

  void Platform::process() {
    Graphics::filled_rectangle(_lcd, 0, 0, 199, 20, Display::Color::eBlack);
    Graphics::write_string(_lcd, "Simon Rocks!", 6, 6, Display::Color::eWhite);
    _lcd->write();
    _lcd->wait();

    Graphics::filled_rectangle(_lcd, 0, 0, 199, 199, Display::Color::eWhite);
    _lcd->write();
    _lcd->wait();

    Graphics::draw_image(_lcd, picard, 0, 0);
    _lcd->write();
    _lcd->wait();
  }
}