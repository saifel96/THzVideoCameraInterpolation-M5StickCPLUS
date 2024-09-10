#include <cstdint>
#ifndef BILNEAR_H
#define BILNEAR_H

#pragma once

class Bilnear {
 public:
  Bilnear();
  ~Bilnear();
  uint8_t *interpolate(uint32_t oldWidth, uint32_t oldHeight,
                       uint8_t *oldPixels, uint32_t newWidth,
                       uint32_t newHeight);

 private:
};

#endif