#ifndef NEARESTNEIGHBOR_H
#define NEARESTNEIGHBOR_H

#include <cstdint>

class NearestNeighbor {
 public:
  NearestNeighbor();
  ~NearestNeighbor();
  uint8_t* interpolate(uint32_t oldWidth, uint32_t oldHeight,
                       uint8_t* oldPixels, uint32_t newWidth,
                       uint32_t newHeight);

 private:
  void nearestNeighborInterpolate(uint8_t* src, int srcWidth, int srcHeight,
                                  uint8_t* dst, int dstWidth, int dstHeight);
};

#endif  // NEARESTNEIGHBOR_H
