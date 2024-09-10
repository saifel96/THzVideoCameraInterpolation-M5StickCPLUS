#include "TemperatureInterpolator.h"

#include <cstddef>  // Fügt die Definition von size_t hinzu
#include <cstring>  // Für memcpy

TemperatureInterpolator::TemperatureInterpolator()
    : search_total(0),
      search_count(0),
      search_lowest(UINT32_MAX),
      search_highest(0) {}

TemperatureInterpolator::~TemperatureInterpolator() {}

uint8_t* TemperatureInterpolator::interpolate(uint32_t oldWidth,
                                              uint32_t oldHeight,
                                              uint8_t* oldPixels,
                                              uint32_t newWidth,
                                              uint32_t newHeight) {
  uint8_t* newPixels = new uint8_t[newWidth * newHeight *
                                   3];  // RGB888 format (3 bytes per pixel)

  // Implementiere hier die Logik zur Anpassung von alten auf neue Dimensionen.

  // Für jetzt kopieren wir die alten Pixel direkt, ohne Interpolation.
  // memcpy(newPixels, oldPixels, oldWidth * oldHeight * 3);

  // Calculate scaling factors
  float x_ratio = (float)oldWidth / newWidth;
  float y_ratio = (float)oldHeight / newHeight;

  for (uint32_t newY = 0; newY < newHeight; ++newY) {
    for (uint32_t newX = 0; newX < newWidth; ++newX) {
      // Map the new pixel position to the old pixel position
      uint32_t oldX = (uint32_t)(newX * x_ratio);
      uint32_t oldY = (uint32_t)(newY * y_ratio);

      // Ensure we do not go out of bounds
      oldX = oldX < oldWidth ? oldX : oldWidth - 1;
      oldY = oldY < oldHeight ? oldY : oldHeight - 1;

      // Calculate the index for old pixels
      size_t oldIndex =
          (oldY * oldWidth + oldX) * 3;  // Each pixel has 3 bytes (RGB)

      // Copy the pixel values from oldPixels to newPixels
      size_t newIndex = (newY * newWidth + newX) * 3;
      newPixels[newIndex] = oldPixels[oldIndex];          // R
      newPixels[newIndex + 1] = oldPixels[oldIndex + 1];  // G
      newPixels[newIndex + 2] = oldPixels[oldIndex + 2];  // B
    }
  }

  // Use interpolateFrame to adjust the pixel values based on temperature data
  uint8_t* diff =
      (uint8_t*)malloc(newWidth * newHeight);  // Example size; adjust as needed
  memset(diff, 0, newWidth * newHeight);       // Initialize diff array

  // Here, you would need the raw pixel data; using newPixels as a placeholder
  interpolateFrame((uint32_t*)newPixels, diff, newWidth, newHeight, 0, newWidth,
                   newHeight, 0, 0);

  // Hier könnte die tatsächliche Interpolationslogik eingefügt werden,
  // um die Temperaturdaten zu verarbeiten.
  free(diff);
  return newPixels;
}

void TemperatureInterpolator::interpolateFrame(uint32_t* pixel_raw,
                                               uint8_t* diff, int frame_width,
                                               int frame_height, int subpage,
                                               int mlx_width, int mlx_height,
                                               int monix, int moniy) {
  // Deine Interpolationslogik bleibt hier unverändert.
  for (int idx = 0; idx < 384; ++idx) {
    uint_fast8_t y = idx >> 4;
    uint_fast8_t x =
        ((mlx_width - 1 - (idx - (y << 4))) << 1) + ((y & 1) != subpage);
    uint_fast16_t xy = x + y * frame_width;

    uint32_t diff_sum = 0;
    size_t count = 0;
    if (x > 0) {
      ++count;
      diff_sum += diff[(xy - 1) >> 1];
    }
    if (x < (frame_width - 1)) {
      ++count;
      diff_sum += diff[(xy + 1) >> 1];
    }
    if (y > 0) {
      ++count;
      diff_sum += diff[(xy - frame_width) >> 1];
    }
    if (y < (frame_height - 1)) {
      ++count;
      diff_sum += diff[(xy + frame_width) >> 1];
    }
    diff_sum /= count;

    int32_t raw = pixel_raw[xy];

    uint32_t sum = 0;
    if (x > 0) {
      sum += pixel_raw[xy - 1];
    }
    if (x < (frame_width - 1)) {
      sum += pixel_raw[xy + 1];
    }
    if (y > 0) {
      sum += pixel_raw[xy - frame_width];
    }
    if (y < (frame_height - 1)) {
      sum += pixel_raw[xy + frame_width];
    }
    raw = (sum + (count >> 1)) / count;

    if (diff_sum > 256) {
      diff_sum = 256;
    }
    raw = (pixel_raw[xy] * (256 - diff_sum) + diff_sum * raw) >> 8;
    pixel_raw[xy] = raw;

    updateMinMaxTemperature(pixel_raw, x, y, frame_width, frame_height,
                            mlx_width, mlx_height, monix, moniy);
  }
}

void TemperatureInterpolator::updateMinMaxTemperature(
    uint32_t* pixel_raw, int x, int y, int frame_width, int frame_height,
    int mlx_width, int mlx_height, int monix, int moniy) {
  uint_fast16_t xy = x + y * frame_width;

  if (((moniy + y - (mlx_height >> 1)) < (moniy << 1)) &&
      ((monix + x - (mlx_width)) < (monix << 1))) {
    search_total += pixel_raw[xy];
    ++search_count;
    if (search_lowest > pixel_raw[xy]) {
      search_lowest = pixel_raw[xy];
    }
    if (search_highest < pixel_raw[xy]) {
      search_highest = pixel_raw[xy];
    }
  }
}
