#include <cstdint>

#ifndef IMAGE_H
#define IMAGE_H

struct Image {
  uint8_t width;
  uint8_t height;
  uint8_t data[];
};

#endif