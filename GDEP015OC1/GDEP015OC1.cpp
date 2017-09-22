/**
 *  Created by Filip Hormot (f.hormot@gmail.com) on 14/09/16.
 */
#include "mbed.h"
#include "GDEP015OC1.h"
#include "5x7.h"

static const unsigned char _lutFull[] = { 
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 
    0x88, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51, 0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

static const unsigned char _lutPart[] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

GDEP015OC1::GDEP015OC1(SPI& spi, PinName cs=p5, PinName dc=p6, PinName rst=p7, PinName busy=p8) : _spi(spi), _cs(cs), _dc(dc), _rst(rst), _busy(busy){
    _bold = true;
    _italic = false;
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
    _rst = _cs = 1;
    empty();    
}

void GDEP015OC1::_wakeUp(bool partial){
    _rst = 0;
    wait_ms(10);
    _rst = 1;
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
    if(partial){
        for(uint8_t i = 0; i<30;i++){
            _spiData(_lutPart[i]);
        }
    } else{
        for(uint8_t i = 0; i<30;i++){
            _spiData(_lutFull[i]);
        }
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

void GDEP015OC1::drawPixel(uint16_t startX, uint16_t startY, bool color)
{
    drawPixel( startX, startY, color?eBlack:eWhite);
}

void GDEP015OC1::drawLine(uint16_t startX,  uint16_t startY, uint16_t stopX, uint16_t stopY, bool color)
{
    drawLine( startX, startY, stopX, stopY, color?eBlack:eWhite);
}
 
void GDEP015OC1::drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, bool color)
{
    drawTriangle( x1, y1, x2, y2, x3, y3, color?eBlack:eWhite);
}

void GDEP015OC1::drawRectangle(uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY, bool color)
{
    drawRectangle( startX, startY, stopX, stopY, color?eBlack:eWhite);
}

void GDEP015OC1::drawCircle(uint16_t startX, uint16_t startY, uint16_t radius, bool color)
{
    drawCircle(startX, startY, radius, color?eBlack:eWhite);
}

void GDEP015OC1::fillCircle(uint16_t startX, uint16_t startY, uint16_t radius, bool color)
{
    fillCircle( startX, startY, radius, color?eBlack:eWhite);
}

void GDEP015OC1::drawEllipse(uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, bool color)
{
    drawEllipse(startX, startY, width, height, color?eBlack:eWhite);
}

void GDEP015OC1::fillEllipse(uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, bool color)
{
    fillEllipse(startX, startY, width, height, color?eBlack:eWhite);
}

void GDEP015OC1::writeChar(char character, uint16_t startX, uint16_t startY, bool color)
{
    writeChar(character, startX, startY, color?eBlack:eWhite);
}

void GDEP015OC1::writeString(char *string, uint16_t startX, uint16_t startY, bool color)
{
    writeString(string, startX, startY, color?eBlack:eWhite);
}

void GDEP015OC1::drawPixel(uint16_t startX, uint16_t startY, Color color=eBlack){
    if(startX>199 || startY>199) return;
    
    uint16_t i = startX/8 + startY*25;

    switch(color) {
        case eWhite :
           _buffer[i] = (_buffer[i] & (0xFF^(1<<(7-startX%8))));
           break;
        case eBlack :
           _buffer[i] = (_buffer[i] | (1<<(7-startX%8)));
           break;   
        case eInvert :
            _buffer[i] = (_buffer[i] ^ (1<<(7-startX%8)));
           break;          
    }
}


void GDEP015OC1::drawLine(uint16_t startX,  uint16_t startY, uint16_t stopX, uint16_t stopY, Color color=eBlack){
    int dx = abs(stopX-startX), sx = startX<stopX ? 1 : -1;
    int dy = abs(stopY-startY), sy = startY<stopY ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;
    
    for(;;){
        drawPixel(startX,startY,color);
        if (startX==stopX && startY==stopY) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; startX += sx; }
        if (e2 < dy) { err += dx; startY += sy; }
    }
}

void GDEP015OC1::drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, Color color=eBlack){
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x3, y3, color);
    drawLine(x3, y3, x1, y1, color);      
}

void GDEP015OC1::drawRectangle(uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY, Color color=eBlack){
    drawLine(startX, startY, stopX,  startY, color);
    drawLine(stopX,  startY, stopX,  stopY,  color);
    drawLine(stopX,  stopY,  startX, stopY,  color);
    drawLine(startX, stopY,  startX, startY, color);    
}

void GDEP015OC1::fillRectangle(uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY, Color color=eBlack){
    for(int x = startX; x<stopX; x++)
    {
        drawLine(x, startY, x,  stopY, color);
    }
    drawLine(startX, startY, stopX,  startY, color);
    drawLine(stopX,  startY, stopX,  stopY,  color);
    drawLine(stopX,  stopY,  startX, stopY,  color);
    drawLine(startX, stopY,  startX, startY, color);    
}


void GDEP015OC1::drawCircle(uint16_t startX, uint16_t startY, uint16_t radius, Color color=eBlack){
    int d,x,y;

    d=3-2*radius;
    x=0;
    y=radius;
    while(x<=y){
        drawPixel(startX+x,startY+y,color);
        drawPixel(startX-y,startY-x,color);
        drawPixel(startX+y,startY-x,color);
        drawPixel(startX-y,startY+x,color);
        drawPixel(startX+y,startY+x,color);
        drawPixel(startX-x,startY-y,color);
        drawPixel(startX+x,startY-y,color);
        drawPixel(startX-x,startY+y,color);

        if(d<=0)
            d=d+4*x+6;
        else{
            d=d+4*x-4*y+10;
            y--;
        }
        x++;
    }
}

void GDEP015OC1::fillCircle(uint16_t startX, uint16_t startY, uint16_t radius, Color color=eBlack){
    for(uint16_t r = 1;r<=radius; r++){
        drawCircle(startX,   startY, r,   color);
        drawCircle(startX+1, startY, r-1, color);
        drawCircle(startX-1, startY, r-1, color);
    }
}

void GDEP015OC1::drawEllipse(uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, Color color=eBlack){
    int a2 = width*width;
    int b2 = height*height;
    int fa2 = 4*a2, fb2 = 4*b2;
    int x, y, sigma;
                                                        
  //First half                                                      
    for(int x = 0, y = height, sigma = 2*b2+a2*(1-2*height); b2*x <= a2*y; x++){
        drawPixel(startX+x,startY+y,color);
        drawPixel(startX-x,startY+y,color);
        drawPixel(startX+x,startY-y,color);
        drawPixel(startX-x,startY-y,color);
        if(sigma >= 0){
            sigma += fa2 * (1-y);
            y--;
        }
        sigma += b2 * ((4 * x) + 6);
    }
    //Second half
    for (x = width, y = 0, sigma = 2*a2+b2*(1-2*width); a2*y <= b2*x; y++){
        drawPixel(startX+x,startY+y,color);
        drawPixel(startX-x,startY+y,color);
        drawPixel(startX+x,startY-y,color);
        drawPixel(startX-x,startY-y,color);
        if (sigma >= 0){
                sigma += fb2 * (1 - x);
                x--;
        }
        sigma += a2 * ((4 * y) + 6);
    }       
}

void GDEP015OC1::fillEllipse(uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, Color color=eBlack){
    for(uint16_t w = width; w > 0; w--){
        drawEllipse(startX, startX, w, height, color);
    }
    drawLine(startX, startY-height, startX, startY+height, color);
}

void GDEP015OC1::writeChar(char character, uint16_t startX, uint16_t startY, Color color=eBlack){
    unsigned char letter[FONT_WIDTH];

    //Grab data for the corresponding font
    for(uint8_t i = 0; i<FONT_WIDTH; i++)
        letter[i] = Font5x7[(character - ' ') * FONT_WIDTH + i];

    for(uint8_t i = 0; i<FONT_WIDTH; i++){
        for(uint8_t j = 0; j<FONT_HEIGHT; j++){
            if((letter[i]>>j)&0x01){
                if(_italic){
                    drawPixel(startX+i+(FONT_HEIGHT/3 - j/3), startY+j, color);
                    if(_bold){
                        for(uint8_t z=0; z<2; z++)
                            drawPixel(startX+i-z+(FONT_HEIGHT/3- j/3), startY+j, color);
                    }
                }
                else{
                    drawPixel(startX+i, startY+j, color);
                    if(_bold){
                        for(uint8_t z=0; z<2; z++)
                            drawPixel(startX+i-z, startY+j, color);
                    }
                }
            }
        }
    }   
}

void GDEP015OC1::writeString(char *string, uint16_t startX, uint16_t startY, Color color=eBlack){
    uint8_t length = 0;
    while(*(string+length) != '\0') length++;
    
    for(uint8_t x=0; x<length; x++)
        writeChar(*(string+x), startX+(FONT_WIDTH+1)*x, startY, color); //FONT_WIDTH+1 gives a 1px space between the characters
}