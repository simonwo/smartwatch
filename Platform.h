#include <memory>
#include "Graphics.h"
#include "Display.h"
#include "Application.h"
#include "ptr.h"

namespace smartwatch {
  class Platform {
   public:
    Platform(ptr<Display>);
    void init();
    void process();

   private:
    ptr<Display> _lcd;
    std::unique_ptr<Application> _current_app;
  };
}