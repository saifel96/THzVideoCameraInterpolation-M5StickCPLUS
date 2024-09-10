#include <M5StickCPlus.h>
#include <image.h>

#include "Bilnear.h"
#include "BiubicInterpolator.hpp"  // Include the selected interpolator
#include "NearestNeighbor.h"
#include "TemperatureInterpolator.h"

#define MAX_DISP_WIDTH 240
#define MAX_DISP_HEIGHT 135

const int origWidth = 32;
const int origHeight = 24;

const int displayWidth = 240;
const int displayHeight = 135;

TFT_eSprite img = TFT_eSprite(&M5.Lcd);
// Bilnear interpolator;
// BiubicInterpolator interpolator;
// NearestNeighbor interpolator;
// OptimizedBilinearInterpolator interpolator;
TemperatureInterpolator interpolator;  // Using the TemperatureInterpolator

void rgb888_to_rgb565_row(uint8_t *rgb888Row, uint16_t *rgb565Row, int width);

int rotationSetting = 1;  // Set rotation: 0 (0°), 1 (90°), 2 (180°), 3 (270°)

void setup() {
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.fillScreen(TFT_BLACK);
  img.setColorDepth(16);
  M5.Lcd.setRotation(rotationSetting);  // Apply the desired rotation
  img.createSprite(displayWidth, displayHeight);
}

uint_fast64_t startTime;
uint_fast64_t endTime;

void loop() {
  startTime = millis();

  img.fillSprite(TFT_BLACK);

  uint8_t *newpixels = interpolator.interpolate(
      origWidth, origHeight, imagePixels, displayWidth, displayHeight);

  if (newpixels == nullptr) {
    img.printf("Interpolation failed!");
    return;
  }

  // Buffer for a single row conversion from RGB888 to RGB565
  uint16_t *rowPixels565 = new uint16_t[displayWidth];

  if (rowPixels565 == nullptr) {
    img.printf("Memory allocation for rowPixels565 failed!");
    delete[] newpixels;  // Clean up to avoid memory leak
    return;
  }

  // Convert and push each row separately
  for (int y = 0; y < displayHeight; y++) {
    rgb888_to_rgb565_row(newpixels + y * displayWidth * 3, rowPixels565,
                         displayWidth);
    img.pushImage(0, displayHeight - y - 1, displayWidth, 1, rowPixels565);
  }

  endTime = millis();
  img.setCursor(10, 10);
  img.setTextColor(TFT_RED);
  img.printf("FPS: %f", 1000.0 / (endTime - startTime));

  img.pushSprite(0, 0);

  delete[] newpixels;
  delete[] rowPixels565;
}

// Optimized function for row-by-row RGB888 to RGB565 conversion
void rgb888_to_rgb565_row(uint8_t *rgb888Row, uint16_t *rgb565Row, int width) {
  for (int i = 0; i < width; i++) {
    int index = i * 3;  // Index for RGB888

    // Get RGB components
    uint8_t b = rgb888Row[index];
    uint8_t g = rgb888Row[index + 1];
    uint8_t r = rgb888Row[index + 2];

    // Convert to RGB565
    rgb565Row[i] = img.color565(r, g, b);
  }
}
