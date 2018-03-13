#include "hardware.h"
#include "GDEP015OC1.h"
#include "Platform.h"

#define SPI_MOSI p11
#define SPI_SCLK p13
#define RESET    p12
#define CS       p14
#define BUSY     p10
#define DC       p8

SPI spi(SPI_MOSI, NC, SPI_SCLK); // mosi, miso, sclk
GDEP015OC1 lcd(spi, CS, DC, RESET, BUSY);
DigitalOut heartbeat_led(LED2);
smartwatch::Platform app(&lcd);

int main() {
    // Configure 8-bit 4-line SPI and
    // Mode 3 - Polarity 1, phase 1,
    // so clock is stable high and data taken on rising edge
    spi.format(8, 3);
    // Set SPI frequency to 3.571 MHz
    // derived from clock cycle time (20->80%) of 250ns + 30ns fall/rise
    spi.frequency(3571E3);

    app.init();

    while (1) {
      app.process();
      sleep();
    }

    heartbeat_led = 1;
}
