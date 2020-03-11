/*
  Mackenzie Malainey 1570494
  CMPUT 275 Winter 2020
  Assignment #2 Restaurants and Pointers
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#include "assets.h"

// physical dimensions of the tft display (# of pixels)
#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320

#define RAND_SEED_LENGTH 32


MCUFRIEND_kbv tft;

void setup() {
  init();
  Serial.begin(9600);

  // tft display initialization
  uint16_t ID = tft.readID();
  tft.begin(ID);

  unsigned long seed = 0;

  // Generate a new random seed at start (gotta keep things *fresh*)
  for (int i = 0; i < 32; i++) {
    seed |= (analogRead(A11) & 1) << i;
  }
  randomSeed(seed);

  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);

}

int main() {
  setup();

  Serial.end();

  return 0;
}
