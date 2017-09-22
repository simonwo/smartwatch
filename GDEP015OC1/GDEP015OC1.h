/**
 *  Created by Filip Hormot (f.hormot@gmail.com) on 14/09/16.
 *  Modified by knutsel.org on 16/07/2017.
 */

#include "mbed.h"

#define BUFFER_SIZE     5000
#define FONT_WIDTH         5
#define FONT_HEIGHT        7
#define BUSY_STATE         1

class GDEP015OC1 {
    public: 
        //unsigned char _buffer[BUFFER_SIZE];
        GDEP015OC1(SPI& spi, PinName cs, PinName dc, PinName rst, PinName busy);
        
        enum Color {eBlack,eWhite,eInvert};
                   
        void fill(unsigned char data, int i);            
        unsigned char get(uint16_t i);            
        void empty(void);            
        void invert(void);            
        void write(void); 
        void writeFull(void);            
 
        //old, put here to not break the API.
        void drawPixel(uint16_t startX, uint16_t startY, bool color);            
        void drawLine(uint16_t startX,  uint16_t startY, uint16_t stopX, uint16_t stopY, bool color);
        void drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, bool color);
        void drawRectangle(uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY, bool color);
        void drawCircle(uint16_t startX, uint16_t startY, uint16_t radius, bool color);
        void fillCircle(uint16_t startX, uint16_t startY, uint16_t radius, bool color);
        void drawEllipse(uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, bool color);
        void fillEllipse(uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, bool color);
        void writeChar(char character, uint16_t startX, uint16_t startY, bool color);
        void writeString(char *string, uint16_t startX, uint16_t startY, bool color);
        //
        
        void drawPixel(uint16_t startX, uint16_t startY, Color color);            
        void drawLine(uint16_t startX,  uint16_t startY, uint16_t stopX, uint16_t stopY, Color color);
        void drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, Color color);
        void drawRectangle(uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY, Color color);
        void fillRectangle(uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY, Color color);
        void drawCircle(uint16_t startX, uint16_t startY, uint16_t radius, Color color);
        void fillCircle(uint16_t startX, uint16_t startY, uint16_t radius, Color color);
        void drawEllipse(uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, Color color);
        void fillEllipse(uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, Color color);
        void writeChar(char character, uint16_t startX, uint16_t startY, Color color);
        void writeString(char *string, uint16_t startX, uint16_t startY, Color color);
    
    private:
        SPI& _spi;
        DigitalOut _cs;
        DigitalOut _dc;
        DigitalOut _rst;
        DigitalIn  _busy;
        
        bool _italic, _bold;
        
        unsigned char _buffer[BUFFER_SIZE];
         
        void _spiCommand(unsigned char command);            
        void _spiData(unsigned char data);            
        void _init(void);
        void _wakeUp(bool partial);            
        void _sleep(void);            
        unsigned char _pixelConv(unsigned char *data, int i);
        uint8_t _mirrorData(uint8_t data);
};