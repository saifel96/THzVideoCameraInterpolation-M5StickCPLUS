
#include <cstdint>
#include <cstdio>

#include "BiubicInterpolator.hpp"

BiubicInterpolator::BiubicInterpolator(/* args */) {}

BiubicInterpolator::~BiubicInterpolator() {}

// Diese Methode führt die bikubische Interpolation durch, um die Größe eines
// Bildes zu ändern. Das Bild wird von den ursprünglichen Abmessungen (oldWidth
// x oldHeight) auf die neuen Abmessungen (newWidth x newHeight) skaliert.

// Rückgabewert: Ein Zeiger auf das neue, interpolierte Bild (newPixels).

uint8_t *BiubicInterpolator::interpolate(uint32_t oldWidth, uint32_t oldHeight,
                                         uint8_t *oldPixels, uint32_t newWidth,
                                         uint32_t newHeight) {
  // Berechnung der Skalierungsfaktoren für die Breite (scaleX) und Höhe
  // (scaleY) des Bildes.
  float scaleX = (float)newWidth / oldWidth;
  float scaleY = (float)newHeight / oldHeight;

  uint8_t *newPixels = new uint8_t[newWidth * newHeight * 3];

  // Diese Schleifen durchlaufen die Pixelzeilen (i) und Spalten (j) im neuen
  // Bildbereich und berechnen die entsprechenden Quellpixelkoordinaten (origX,
  // origY) im Originalbild. Die Schleife für i beginnt unten und arbeitet sich
  // nach oben, um die y-Achse anzupassen.

  for (int32_t i = 0; i < newHeight;
       i++) {  // Start from the lowest row and go up
    for (uint32_t j = 0; j < newWidth; j++) {
      int origX = (int)(j / scaleX);
      int origY = (int)(i / scaleY);
      // Adjust for inverted y-axis
      // Diese Bedingung stellt sicher, dass die Quellpixelkoordinaten im
      // gültigen Bereich des Originalbildes liegen. Die Funktion verwendet dann
      // bikubische Interpolation.
      if (origY >= 1 && origY < oldHeight - 2 && origX >= 1 &&
          origX < oldWidth - 2) {
        // Diese Schleifen berechnen den neuen Farbwert (Rot, Grün, Blau) für
        // jedes Pixel im neuen Bild durch bikubische Interpolation. Die
        // Farbwerte der umgebenden 16 Pixel im Originalbild werden verwendet,
        // um die neuen Farbwerte zu berechnen.
        // Die Methode cubicInterpolate() wird aufgerufen, um den bikubischen
        // Interpolationswert für die x- und y-Achsenverschiebungen (dx, dy) zu
        // berechnen.

        for (int k = 0; k < 3; k++) {
          float sum = 0.0;
          for (int m = -1; m <= 2; m++) {
            for (int n = -1; n <= 2; n++) {
              float dx = (j / scaleX) - origX;
              float dy = (i / scaleY) - origY;  // Adjust for inverted y-axis
              float bx = cubicInterpolate(dx - n);
              float by = cubicInterpolate(dy - m);
              int pixelIndex = ((origY + m) * oldWidth + (origX + n)) * 3 + k;
              sum += bx * by * oldPixels[pixelIndex];
            }
          }

          int newPixelIndex = (i * newWidth + j) * 3 + k;
          newPixels[newPixelIndex] =
              sum > 255.0f ? 255 : (sum < 0.0f ? 0 : (uint8_t)sum);
        }
        // Wenn die Quellpixelkoordinaten außerhalb des gültigen Bereichs
        // liegen, wird der Farbwert des nächstgelegenen gültigen Pixels
        // verwendet, um den neuen Pixelwert zu setzen.
      } else {
        for (int k = 0; k < 3; k++) {
          int pixelIndex = (origY * oldWidth + origX) * 3 + k;
          int newPixelIndex = (i * newWidth + j) * 3 + k;
          newPixels[newPixelIndex] = oldPixels[pixelIndex];
        }
      }
    }
  }

  return newPixels;
}
// Diese Methode berechnet den bikubischen Interpolationswert basierend auf der
// gegebenen x-Verschiebung. Sie verwendet eine Formel, die auf dem Abstand vom
// Ursprungswert basiert.
float BiubicInterpolator::cubicInterpolate(float x) {
  // Berechnung des absoluten Wertes von x sowie seines Quadrats (absX2) und
  // seiner dritten Potenz (absX3).
  float absX = x < 0 ? -x : x;
  float absX2 = absX * absX;
  float absX3 = absX2 * absX;

  // Diese Bedingungen berechnen den interpolierten Wert basierend auf dem
  // Abstand x. Für absX ≤ 1 wird eine Formel verwendet, die den Wert für nähere
  // Pixel höher gewichtet. Für 1 < absX ≤ 2 wird eine andere Formel verwendet,
  // die den Einfluss von weiter entfernten Pixeln verringert. Wenn absX > 2,
  // wird 0.0f zurückgegeben, da die Pixel zu weit entfernt sind, um relevant zu
  // sein.
  if (absX <= 1.0f) {
    return (1.5f * absX3 - 2.5f * absX2 + 1.0f);
  } else if (absX <= 2.0f) {
    return (-0.5f * absX3 + 2.5f * absX2 - 4.0f * absX + 2.0f);
  }

  return 0.0f;
}
