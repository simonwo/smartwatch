#include "Graphics.h"

extern const Image picard;

namespace smartwatch {
  template<class Display>
  class Platform {
   public:
    Platform(Display);
    void init();
    void process();

   private:
    Display _lcd;
  };

  template<class Display>
  Platform<Display>::Platform(Display lcd)
  : _lcd(lcd) {
  }

  template<class Display>
  void Platform<Display>::init() {
  }

  template<class Display>
  void Platform<Display>::process() {
    Graphics::filled_rectangle(_lcd, 0, 0, 199, 20, Display::eBlack);
    Graphics::write_string(_lcd, "Simon Rocks!", 6, 6, Display::eWhite);
    _lcd.write();
    _lcd.wait();

    Graphics::filled_rectangle(_lcd, 0, 0, 199, 199, Display::eWhite);
    _lcd.write();
    _lcd.wait();

    Graphics::draw_image(_lcd, picard, 0, 0);
    _lcd.write();
    _lcd.wait();
  }
}