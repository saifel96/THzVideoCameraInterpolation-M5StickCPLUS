#include "Bilnear.h"

#include <math.h>

#include <algorithm>
#include <thread>
#include <vector>

Bilnear::Bilnear() {}

Bilnear::~Bilnear() {}
// Diese Funktion führt die bilineare Interpolation für einen bestimmten
// Abschnitt des Bildes durch. Sie berechnet die neuen Pixelwerte basierend auf
// den Pixeln im Originalbild.

void interpolateSection(uint32_t oldWidth, uint32_t oldHeight,
                        uint8_t *oldPixels, uint32_t newWidth,
                        uint32_t newHeight, uint8_t *newPixels, float invScaleX,
                        float invScaleY, uint32_t startRow, uint32_t endRow) {
  // Diese Schleifen durchlaufen die Pixelzeilen (i) und Spalten (j) im neuen
  // Bildbereich. Dabei wird der entsprechende Quellpixelort im Originalbild
  // berechnet.
  for (uint32_t i = startRow; i < endRow; ++i) {
    uint32_t yi = i * newWidth * 3;
    // float srcYFloat = (newHeight - 1 - i) * invScaleY;
    float srcYFloat = (i * invScaleY);
    int srcY = static_cast<int>(srcYFloat);

    for (uint32_t j = 0; j < newWidth; ++j) {
      float srcXFloat = j * invScaleX;
      int srcX = static_cast<int>(srcXFloat);
      // Diese Bedingung stellt sicher, dass die berechneten Quellpixel
      // innerhalb der Grenzen des Originalbildes liegen. x_diff und y_diff
      // repräsentieren den Abstand zu den nächsten Pixeln.
      if (srcY >= 0 && srcY < oldHeight - 1 && srcX >= 0 &&
          srcX < oldWidth - 1) {
        float x_diff = srcXFloat - srcX;
        float y_diff = srcYFloat - srcY;
        // Berechnung der Indizes der vier benachbarten Pixel (A, B, C, D) im
        // Originalbild, die zur Berechnung des neuen Pixelwerts verwendet
        // werden.
        int indexA = (srcY * oldWidth + srcX) * 3;
        int indexB = indexA + 3;
        int indexC = ((srcY + 1) * oldWidth + srcX) * 3;
        int indexD = indexC + 3;
        // Diese Schleife interpoliert den neuen Farbwert (Rot, Grün, Blau)
        // basierend auf den Farbwerten der vier benachbarten Pixel im
        // Originalbild. Die neuen Werte werden in newPixels gespeichert.
        for (int k = 0; k < 3; ++k) {
          uint8_t A = oldPixels[indexA + k];
          uint8_t B = oldPixels[indexB + k];
          uint8_t C = oldPixels[indexC + k];
          uint8_t D = oldPixels[indexD + k];

          newPixels[yi + j * 3 + k] = static_cast<uint8_t>(
              A * (1 - x_diff) * (1 - y_diff) + B * x_diff * (1 - y_diff) +
              C * (1 - x_diff) * y_diff + D * x_diff * y_diff);
        }
        // Wenn der Quellpixelort außerhalb des Bildbereichs liegt, wird der
        // Farbwert des nächstgelegenen gültigen Pixels verwendet, um den neuen
        // Pixelwert zu setzen.
      } else {
        int edgeIndex = (srcY * oldWidth + srcX) * 3;
        for (int k = 0; k < 3; ++k) {
          newPixels[yi + j * 3 + k] = oldPixels[edgeIndex + k];
        }
      }
    }
  }
}
// Hauptfunktion zur Durchführung der bilinearen Interpolation. Sie skaliert das
// Bild von den ursprünglichen Abmessungen (oldWidth x oldHeight) auf die neuen
// Abmessungen (newWidth x newHeight).
// Rückgabewert: Ein Zeiger auf das neue, interpolierte Bild (newPixels).
uint8_t *Bilnear::interpolate(uint32_t oldWidth, uint32_t oldHeight,
                              uint8_t *oldPixels, uint32_t newWidth,
                              uint32_t newHeight) {
  // Berechnung der Skalierungsfaktoren für die Breite (scaleX) und Höhe
  // (scaleY) des Bildes.
  float scaleX = static_cast<float>(newWidth) / oldWidth;
  float scaleY = static_cast<float>(newHeight) / oldHeight;

  uint8_t *newPixels = new uint8_t[newWidth * newHeight * 3];

  float invScaleX = 1.0f / scaleX;
  float invScaleY = 1.0f / scaleY;

  // Ermittlung der Anzahl der Threads, die für die Interpolation verwendet
  // werden sollen, basierend auf der verfügbaren Hardware. rowsPerThread teilt
  // das Bild in Abschnitte, die parallel verarbeitet werden.

  unsigned int numThreads = std::min(std::thread::hardware_concurrency(), 8U);
  std::vector<std::thread> threads;
  uint32_t rowsPerThread = newHeight / numThreads;

  // Erstellung und Start der Threads, die jeweils einen Abschnitt des Bildes
  // interpolieren. Jeder Thread bearbeitet eine bestimmte Anzahl von Zeilen
  // (startRow bis endRow).
  for (unsigned int t = 0; t < numThreads; ++t) {
    uint32_t startRow = t * rowsPerThread;
    uint32_t endRow =
        (t == numThreads - 1) ? newHeight : (t + 1) * rowsPerThread;
    threads.emplace_back(interpolateSection, oldWidth, oldHeight, oldPixels,
                         newWidth, newHeight, newPixels, invScaleX, invScaleY,
                         startRow, endRow);
  }
  // Wartet darauf, dass alle gestarteten Threads ihre Arbeit beendet haben.
  for (auto &thread : threads) {
    thread.join();
  }
  // Gibt das neue Bild als Zeiger auf die interpolierten Pixel zurück.
  return newPixels;
}
