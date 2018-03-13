#include "Graphics.h"
#include "Display.h"
#include "ptr.h"

namespace smartwatch {
  class Platform {
   public:
    Platform(ptr<Display>);
    void init();
    void process();

   private:
    ptr<Display> _lcd;
  };
}