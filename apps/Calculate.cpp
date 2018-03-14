#include "Application.h"
#include "Calculate.h"
#include "Display.h"
#include "Graphics.h"

namespace smartwatch {
  namespace apps {
    Calculate::Calculate(ptr<Display> display) : Application(display) {
      Graphics::rectangle(display_, 4, 4, 196, 14);
    }

    Application::RunState Calculate::process() {
      Graphics::write_string(display_, "0", 6, 6);
      display_->write();
      display_->wait();
      return Application::RunState::Snooze;
    }
  }
}