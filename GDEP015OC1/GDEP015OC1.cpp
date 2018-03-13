/**
 *  Created by Filip Hormot (f.hormot@gmail.com) on 14/09/16.
 *  Refactored by Simon Worthington on 22/09/2017.
 */
#include "hardware.h"
#include "GDEP015OC1.h"
#include "Display.h"

using smartwatch::Display;

static const unsigned char _lutFull[] = {
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99,
    0x88, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51, 0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

static const unsigned char _lutPart[] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

GDEP015OC1::GDEP015OC1(SPI& spi, PinName cs=p5, PinName dc=p6, PinName rst=p7, PinName busy=p8) : _spi(spi), _cs(cs), _dc(dc), _reset(rst), _busy(busy){
    _init();
}

void GDEP015OC1::_spiCommand(unsigned char command){
    _cs = _dc = 0;
    wait_us(1);

    _spi.write(command);
}

void GDEP015OC1::_spiData(unsigned char data){
    _cs = 0;
    _dc = 1;
    wait_us(1);

    _spi.write(data);
}

void GDEP015OC1::_init(void){
    _reset = _cs = 1;
    empty();
}

void GDEP015OC1::wait() {
    while (_busy == BUSY_STATE);
}

void GDEP015OC1::_wakeUp(bool partial){
    _reset = 0;
    wait_ms(10);
    _reset = 1;
    wait_ms(10);

    //Stage 3
    //Driver Output control
    _spiCommand(0x01);
    _spiData(0xC7);
    _spiData(0x00);
    _spiData(0x00);
    //Softstart
    _spiCommand(0x0C);
    _spiData(0xD7);
    _spiData(0xD6);
    _spiData(0x9D);

    _spiCommand(0x22);
    _spiData(0xF0);

    //VCOM setting
    _spiCommand(0x2C);
    _spiData(0xA8);
    //Dummy line period
    _spiCommand(0x3A);
    _spiData(0x1A);
    //Gate line width
    _spiCommand(0x3B);
    _spiData(0x08);
    //Set data entry mode
    _spiCommand(0x11);
    _spiData(0x01); //DO NOT TOUCH THIS! (MUST BE 0x01)

    //Gate driver start position
    _spiCommand(0x0F);
    _spiData(0x2B);
    _spiData(0x01);

    //VGH VGL
    _spiCommand(0x03);
    _spiData(0x00);
    //VSH, VSL
    _spiCommand(0x04);
    _spiData(0x0A);

    //Define X display size
    _spiCommand(0x44);
    _spiData(0x00);
    _spiData(0x18);
    //Define Y display size
    _spiCommand(0x45);
    _spiData(0xC7);
    _spiData(0x00);
    _spiData(0x2B);
    _spiData(0x01);

    //Define X RAM address
    _spiCommand(0x4E);
    _spiData(0x00);
    //Define Y RAM address
    _spiCommand(0x4F);
    _spiData(0xC7);
    _spiData(0x00);

    //Border waveform control
    _spiCommand(0x3C);
    _spiData(0x33);

    //Write LUT
    _spiCommand(0x32);
    auto lut = partial ? _lutPart : _lutFull;
    for(uint8_t i = 0; i<30;i++){
        _spiData(lut[i]);
    }

    //Power on
    _spiCommand(0x22);
    _spiData(0xC0);

    //Define X RAM address
    _spiCommand(0x4E);
    _spiData(0x00);
    //Define Y RAM address
    _spiCommand(0x4F);
    _spiData(0xC7);
    _spiData(0x00);
}

void GDEP015OC1::_sleep(void){
    while(_busy == BUSY_STATE);

    _spiCommand(0x10);
    _spiData(0x01);
}

/*
 *  If you touch this function satan will feast on your soul for an eternity!
 *  IM NOT PLAYING AROUND DONT FUCKING TOUCH IT!
 *  You are thinking about it...
 *  .
 *  .
 *  .
 *  DONT!
 */
unsigned char GDEP015OC1::_pixelConv(unsigned char *data, int i){
    uint8_t pix = 0x00;
    for(uint8_t x = 0; x < 8; x++){
        pix |= ((*(data + (i*200)%5000 + (24-i/200) + x*25)>>((i/25)%8))&(0x01))<<(7-x);
    }
    return pix^0xFF;
}

uint8_t GDEP015OC1::_mirrorData(uint8_t data){
    uint8_t mirror = 0x00;
    for(uint8_t i=0; i<8; i++)
        mirror |= ((data>>i) & 0x01) << (7 - i);

    return mirror;
}

void GDEP015OC1::fill(unsigned char data, int x){
    _buffer[x] = data;
}

void GDEP015OC1::empty(void){
    for(uint16_t x=0; x<5000; x++)
        _buffer[x] = 0x00;
}

void GDEP015OC1::write(void){
    _wakeUp(true);

    _spiCommand(0x24);
    for(int16_t x=0; x>=0 && x<200; x++){
        for(int16_t y=24; y>=0 && y<25; y--){
            _spiData(_mirrorData(_pixelConv(_buffer, x*25+y)));
            wait_us(10);
        }
    }

    _spiCommand(0x22);
    _spiData(0x04);
    _spiCommand(0x22);
    _spiData(0x08);

    //Update
    _spiCommand(0x22);
    _spiData(0xC7);
    _spiCommand(0x20);

    _sleep();
}

void GDEP015OC1::writeFull(void){
    _wakeUp(false);

    _spiCommand(0x24);
    for(int16_t x=0; x>=0 && x<200; x++){
        for(int16_t y=24; y>=0 && y<25; y--){
            _spiData(_mirrorData(_pixelConv(_buffer, x*25+y)));
            wait_us(10);
        }
    }

    _spiCommand(0x22);
    _spiData(0x04);
    _spiCommand(0x22);
    _spiData(0x08);

    //Update
    _spiCommand(0x22);
    _spiData(0xC7);
    _spiCommand(0x20);

    _sleep();
}

void GDEP015OC1::draw(uint16_t x, uint16_t y, Color color=Color::eBlack){
    if(x>199 || y>199) return;

    uint16_t i = x/8 + y*25;

    switch(color) {
        case Color::eWhite :
           _buffer[i] = (_buffer[i] & (0xFF^(1<<(7-x%8))));
           break;
        case Color::eBlack :
           _buffer[i] = (_buffer[i] | (1<<(7-x%8)));
           break;
        case Color::eInvert :
            _buffer[i] = (_buffer[i] ^ (1<<(7-x%8)));
           break;
    }
}
