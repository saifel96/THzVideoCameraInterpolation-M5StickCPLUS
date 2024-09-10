class BiubicInterpolator {
 private:
  /* data */
 public:
  BiubicInterpolator();
  ~BiubicInterpolator();

  uint8_t *interpolate(uint32_t oldWidth, uint32_t oldHeight,
                       uint8_t *oldPixels, uint32_t newWidth,
                       uint32_t newHeight);
  double cubicInterpolate(double t);
  float cubicInterpolate(float t);
};
