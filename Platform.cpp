#include "Platform.h"
#include "Display.h"
#include "Graphics.h"
#include "Application.h"
#include "apps/Calculate.h"
#include "ptr.h"

extern const Image picard;

namespace smartwatch {
  Platform::Platform(ptr<Display> lcd)
  : _lcd(lcd), _current_app(nullptr) {
  }

  void Platform::init() {
    Graphics::filled_rectangle(_lcd, 0, 0, 200, 200, Display::Color::eWhite);
    _current_app.reset(new apps::Calculate(_lcd));
  }

  void Platform::process() {
    const auto app_state = _current_app->process();
    switch (app_state) {
      case Application::RunState::Snooze:
        break;
      case Application::RunState::Exit:
        _current_app.reset(new apps::Calculate(_lcd));
        break;
    }
  }
}