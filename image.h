#include <cstdint>

struct Image {
  uint8_t width;
  uint8_t height;
  uint8_t data[];
};