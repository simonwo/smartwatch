#include "hardware.h"
#include "GDEP015OC1.h"
#include "Graphics.h"

#define SPI_MOSI p11
#define SPI_SCLK p13
#define RESET    p12
#define CS       p14
#define BUSY     p10
#define DC       p8

extern const Image picard;

SPI spi(SPI_MOSI, NC, SPI_SCLK); // mosi, miso, sclk
GDEP015OC1 lcd(spi, CS, DC, RESET, BUSY);

//DigitalOut reset(p12);
//DigitalOut chip_select(p14);
//DigitalOut
DigitalIn busy(p10);

DigitalOut busy_led(LED1);
DigitalOut heartbeat_led(LED2);

int main() {
    // Configure 8-bit 4-line SPI and
    // Mode 3 - Polarity 1, phase 1,
    // so clock is stable high and data taken on rising edge
    spi.format(8, 3);
    // Set SPI frequency to 3.571 MHz
    // derived from clock cycle time (20->80%) of 250ns + 30ns fall/rise
    spi.frequency(3571E3);
    
    Graphics::filled_rectangle(lcd, 0, 0, 199, 20, GDEP015OC1::eBlack);
    Graphics::write_string(lcd, "Simon Rocks!", 6, 6, GDEP015OC1::eWhite);
    lcd.write();
    lcd.wait();

    Graphics::filled_rectangle(lcd, 0, 0, 199, 199, GDEP015OC1::eWhite);
    lcd.write();
    lcd.wait();

    Graphics::draw_image(lcd, picard, 0, 0);
    lcd.write();
    lcd.wait();
    
    heartbeat_led = 1;
}
