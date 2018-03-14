#include "Application.h"
#include "Display.h"
#include "ptr.h"

namespace smartwatch {
  Application::Application(ptr<Display> display) : display_(display) {
  }

  Application::RunState Application::process() {
    return RunState::Exit;
  }
}