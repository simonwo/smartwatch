#include "mbed.h"
#include "GDEP015OC1.h"

#define SPI_MOSI p11
#define SPI_SCLK p13
#define RESET    p12
#define CS       p14
#define BUSY     p10
#define DC       p8

SPI spi(SPI_MOSI, NC, SPI_SCLK); // mosi, miso, sclk
GDEP015OC1 lcd(spi, CS, DC, RESET, BUSY);

//DigitalOut reset(p12);
//DigitalOut chip_select(p14);
//DigitalOut
DigitalIn busy(p10);

DigitalOut busy_led(LED1);
DigitalOut heartbeat_led(LED2);

#define SPI_COMMAND(X) (X)
#define SPI_DATA(X)    (0b10000000 | X)

#define CMD_DEEP_SLEEP SPI_COMMAND(0x10)
#define CMD_SW_RESET   SPI_COMMAND(0x12)

int main() {
    // Configure 8-bit 4-line SPI and
    // Mode 3 - Polarity 1, phase 1,
    // so clock is stable high and data taken on rising edge
    spi.format(8, 3);
    // Set SPI frequency to 3.571 MHz
    // derived from clock cycle time (20->80%) of 250ns + 30ns fall/rise
    spi.frequency(3571E3);
    
    lcd.writeString("Simon", 20, 20, GDEP015OC1::eBlack);
    lcd.writeString("Rocks!", 20, 30, GDEP015OC1::eBlack);
    lcd.write();
    while(busy);
    lcd.fillRectangle(60, 20, 80, 180, GDEP015OC1::eBlack);
    lcd.write();
    while(busy);
    lcd.fillRectangle(0, 0, 199, 199, GDEP015OC1::eBlack);
    lcd.write();
    while(busy);
    lcd.fillRectangle(0, 0, 199, 199, GDEP015OC1::eWhite);
    lcd.write();
    
    heartbeat_led = 1;
}
