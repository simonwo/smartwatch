#include "Application.h"
#include "Display.h"

#ifndef CALCULATE_H
#define CALCULATE_H

namespace smartwatch {
  namespace apps {
    class Calculate : public Application {
     public:
      Calculate(ptr<Display>);
      Application::RunState process() override;
    };
  }
}

#endif
