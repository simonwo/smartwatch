#include "5x7.h"

static const int FONT_WIDTH = 5;
static const int FONT_HEIGHT = 7;

namespace Graphics {
  template <typename Display>
  void line(Display& display, uint16_t startX,  uint16_t startY, uint16_t stopX, uint16_t stopY, typename Display::Color color=Display::eBlack){
      int dx = abs(stopX-startX), sx = startX<stopX ? 1 : -1;
      int dy = abs(stopY-startY), sy = startY<stopY ? 1 : -1;
      int err = (dx>dy ? dx : -dy)/2, e2;

      for(;;){
          display.draw(startX,startY,color);
          if (startX==stopX && startY==stopY) break;
          e2 = err;
          if (e2 >-dx) { err -= dy; startX += sx; }
          if (e2 < dy) { err += dx; startY += sy; }
      }
  }

  template <typename Display>
  void triangle(Display& display, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, typename Display::Color color=Display::eBlack){
      line(display, x1, y1, x2, y2, color);
      line(display, x2, y2, x3, y3, color);
      line(display, x3, y3, x1, y1, color);
  }

  template <typename Display>
  void rectangle(Display& display, uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY, typename Display::Color color=Display::eBlack){
      line(display, startX, startY, stopX,  startY, color);
      line(display, stopX,  startY, stopX,  stopY,  color);
      line(display, stopX,  stopY,  startX, stopY,  color);
      line(display, startX, stopY,  startX, startY, color);
  }

  template <typename Display>
  void filled_rectangle(Display& display, uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY, typename Display::Color color=Display::eBlack){
      for(int x = startX; x<stopX; x++)
      {
          line(display, x, startY, x,  stopY, color);
      }
      line(display, startX, startY, stopX,  startY, color);
      line(display, stopX,  startY, stopX,  stopY,  color);
      line(display, stopX,  stopY,  startX, stopY,  color);
      line(display, startX, stopY,  startX, startY, color);
  }


  template <typename Display>
  void circle(Display& display, uint16_t startX, uint16_t startY, uint16_t radius, typename Display::Color color=Display::eBlack){
      int d,x,y;

      d=3-2*radius;
      x=0;
      y=radius;
      while(x<=y){
          display.draw(startX+x,startY+y,color);
          display.draw(startX-y,startY-x,color);
          display.draw(startX+y,startY-x,color);
          display.draw(startX-y,startY+x,color);
          display.draw(startX+y,startY+x,color);
          display.draw(startX-x,startY-y,color);
          display.draw(startX+x,startY-y,color);
          display.draw(startX-x,startY+y,color);

          if(d<=0)
              d=d+4*x+6;
          else{
              d=d+4*x-4*y+10;
              y--;
          }
          x++;
      }
  }

  template <typename Display>
  void filled_circle(Display& display, uint16_t startX, uint16_t startY, uint16_t radius, typename Display::Color color=Display::eBlack){
      for(uint16_t r = 1;r<=radius; r++){
          circle(display, startX,   startY, r,   color);
          circle(display, startX+1, startY, r-1, color);
          circle(display, startX-1, startY, r-1, color);
      }
  }

  template <typename Display>
  void ellipse(Display& display, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, typename Display::Color color=Display::eBlack){
      int a2 = width*width;
      int b2 = height*height;
      int fa2 = 4*a2, fb2 = 4*b2;
      int x, y, sigma;

    //First half
      for(int x = 0, y = height, sigma = 2*b2+a2*(1-2*height); b2*x <= a2*y; x++){
          display.draw(startX+x,startY+y,color);
          display.draw(startX-x,startY+y,color);
          display.draw(startX+x,startY-y,color);
          display.draw(startX-x,startY-y,color);
          if(sigma >= 0){
              sigma += fa2 * (1-y);
              y--;
          }
          sigma += b2 * ((4 * x) + 6);
      }
      //Second half
      for (x = width, y = 0, sigma = 2*a2+b2*(1-2*width); a2*y <= b2*x; y++){
          display.draw(startX+x,startY+y,color);
          display.draw(startX-x,startY+y,color);
          display.draw(startX+x,startY-y,color);
          display.draw(startX-x,startY-y,color);
          if (sigma >= 0){
                  sigma += fb2 * (1 - x);
                  x--;
          }
          sigma += a2 * ((4 * y) + 6);
      }
  }

  template <typename Display>
  void filled_ellipse(Display& display, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, typename Display::Color color=Display::eBlack){
      for(uint16_t w = width; w > 0; w--){
          ellipse(display, startX, startX, w, height, color);
      }
      line(display, startX, startY-height, startX, startY+height, color);
  }

  template <typename Display>
  void write_char(Display& display, char character, uint16_t startX, uint16_t startY, typename Display::Color color=Display::eBlack, bool italic=false, bool bold=false){
      unsigned char letter[FONT_WIDTH];

      //Grab data for the corresponding font
      for(uint8_t i = 0; i<FONT_WIDTH; i++)
          letter[i] = Font5x7[(character - ' ') * FONT_WIDTH + i];

      for(uint8_t i = 0; i<FONT_WIDTH; i++){
          for(uint8_t j = 0; j<FONT_HEIGHT; j++){
              if((letter[i]>>j)&0x01){
                  if(italic){
                      display.draw(startX+i+(FONT_HEIGHT/3 - j/3), startY+j, color);
                      if(bold){
                          for(uint8_t z=0; z<2; z++)
                              display.draw(startX+i-z+(FONT_HEIGHT/3- j/3), startY+j, color);
                      }
                  }
                  else{
                      display.draw(startX+i, startY+j, color);
                      if(bold){
                          for(uint8_t z=0; z<2; z++)
                              display.draw(startX+i-z, startY+j, color);
                      }
                  }
              }
          }
      }
  }

  template <typename Display>
  void write_string(Display& display, char *string, uint16_t startX, uint16_t startY, typename Display::Color color=Display::eBlack, bool italic=false, bool bold=false){
      uint8_t length = 0;
      while(*(string+length) != '\0') length++;

      for(uint8_t x=0; x<length; x++)
          write_char(display, *(string+x), startX+(FONT_WIDTH+1)*x, startY, color, italic, bold); //FONT_WIDTH+1 gives a 1px space between the characters
  }
}