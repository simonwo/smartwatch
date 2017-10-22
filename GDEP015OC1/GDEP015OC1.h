/**
 *  Created by Filip Hormot (f.hormot@gmail.com) on 14/09/16.
 *  Modified by knutsel.org on 16/07/2017.
 *  Refactored by Simon Worthington on 22/09/2017.
 */

#include "hardware.h"
#define BUSY_STATE         1

class GDEP015OC1 {
    public:
        GDEP015OC1(SPI& spi, PinName cs, PinName dc, PinName rst, PinName busy);

        enum Color {eBlack,eWhite,eInvert};

        void fill(unsigned char data, int i);
        unsigned char get(uint16_t i);
        void empty();
        void invert();
        void write();
        void writeFull();
        void wait();

        void draw(uint16_t x, uint16_t y, Color color);

    private:
        SPI& _spi;
        DigitalOut _cs;
        DigitalOut _dc;
        DigitalOut _reset;
        DigitalIn  _busy;

        static const uint16_t BUFFER_SIZE = 5000;
        unsigned char _buffer[BUFFER_SIZE];

        void _spiCommand(unsigned char command);
        void _spiData(unsigned char data);
        void _init();
        void _wakeUp(bool partial);
        void _sleep();
        unsigned char _pixelConv(unsigned char *data, int i);
        uint8_t _mirrorData(uint8_t data);
};
