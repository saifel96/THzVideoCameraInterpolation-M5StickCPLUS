#include "NearestNeighbor.h"

NearestNeighbor::NearestNeighbor() {}

NearestNeighbor::~NearestNeighbor() {}

void NearestNeighbor::nearestNeighborInterpolate(uint8_t* src, int srcWidth,
                                                 int srcHeight, uint8_t* dst,
                                                 int dstWidth, int dstHeight) {
  float xRatio = float(srcWidth) / dstWidth;
  float yRatio = float(srcHeight) / dstHeight;
  int nearestX, nearestY;
  for (int i = 0; i < dstHeight; i++) {
    nearestY = int(i * yRatio);
    for (int j = 0; j < dstWidth; j++) {
      nearestX = int(j * xRatio);
      int srcIndex = (nearestY * srcWidth + nearestX) * 3;
      int dstIndex = (i * dstWidth + j) * 3;
      dst[dstIndex] = src[srcIndex];
      dst[dstIndex + 1] = src[srcIndex + 1];
      dst[dstIndex + 2] = src[srcIndex + 2];
    }
  }
}

uint8_t* NearestNeighbor::interpolate(uint32_t oldWidth, uint32_t oldHeight,
                                      uint8_t* oldPixels, uint32_t newWidth,
                                      uint32_t newHeight) {
  uint8_t* newPixels = new uint8_t[newWidth * newHeight * 3];
  nearestNeighborInterpolate(oldPixels, oldWidth, oldHeight, newPixels,
                             newWidth, newHeight);
  return newPixels;
}
