#include "Display.h"
#include "ptr.h"

#ifndef APPLICATION_H
#define APPLICATION_H

namespace smartwatch {
  class Application {
   public:
    enum class RunState {
      Snooze,
      Exit
    };

    Application(ptr<Display>);
    virtual RunState process();

   protected:
    ptr<Display> display_;
  };
}

#endif
