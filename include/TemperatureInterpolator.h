#ifndef TEMPERATURE_INTERPOLATOR_H
#define TEMPERATURE_INTERPOLATOR_H

#include <cstdint>

class TemperatureInterpolator {
 public:
  TemperatureInterpolator();
  ~TemperatureInterpolator();

  uint8_t* interpolate(uint32_t oldWidth, uint32_t oldHeight,
                       uint8_t* oldPixels, uint32_t newWidth,
                       uint32_t newHeight);

 private:
  void interpolateFrame(uint32_t* pixel_raw, uint8_t* diff, int frame_width,
                        int frame_height, int subpage, int mlx_width,
                        int mlx_height, int monix, int moniy);
  void updateMinMaxTemperature(uint32_t* pixel_raw, int x, int y,
                               int frame_width, int frame_height, int mlx_width,
                               int mlx_height, int monix, int moniy);

  int search_total;
  int search_count;
  uint32_t search_lowest;
  uint32_t search_highest;
};

#endif  // TEMPERATURE_INTERPOLATOR_H
