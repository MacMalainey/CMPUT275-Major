/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include <Arduino.h>
#include <math.h>

#include "include/misc.h"

// https://gist.github.com/kuathadianto/200148f53616cbd226d993b400214a7fs
uint16_t HSVtoRGB(int H, double S, double V) {
  double C = S * V;
  double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
  double m = V - C;
  double Rs, Gs, Bs;

  if (H >= 0 && H < 60) {
    Rs = C;
    Gs = X;
    Bs = 0;
  } else if (H >= 60 && H < 120) {
    Rs = X;
    Gs = C;
    Bs = 0;
  } else if (H >= 120 && H < 180) {
    Rs = 0;
    Gs = C;
    Bs = X;
  } else if (H >= 180 && H < 240) {
    Rs = 0;
    Gs = X;
    Bs = C;
  } else if (H >= 240 && H < 300) {
    Rs = X;
    Gs = 0;
    Bs = C;
  } else {
    Rs = C;
    Gs = 0;
    Bs = X;
  }

  uint16_t rgb = 0;

  rgb += (uint16_t)((Rs + m) * 31) << 11;  // Bit shift to bytes 11 - 15
  rgb += (uint16_t)((Gs + m) * 63) << 5;   // Bit shift to bytes 5 - 10
  rgb += (uint16_t)((Bs + m) * 31);        // stays at bytes 0 - 4
  return rgb;
}

uint16_t genNeonColor() { return HSVtoRGB(random(256), 1.0, 1.0); }