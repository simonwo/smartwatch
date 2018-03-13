#include "SDLDisplay.h"
#include "Platform.h"

int main(int, char**) {
    SDL_Init(SDL_INIT_VIDEO);
    smartwatch::simulator::SDLDisplay lcd(3);
    smartwatch::Platform app(&lcd);

    app.init();
    app.process();

    SDL_Delay(5000);

    SDL_Quit();
    return 0;
}

