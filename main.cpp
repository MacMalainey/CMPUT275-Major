/*
  Mackenzie Malainey 1570494
  CMPUT 275 Winter 2020
  Assignment #2 Restaurants and Pointers
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#include "assets.h"
#include "map.h"

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

  uint32_t seed = 0;

  // Generate a new random seed at start (gotta keep things *fresh*)
  for (int i = 0; i < RAND_SEED_LENGTH; i++) {
    seed |= (analogRead(A11) & 1) << i;
  }
  randomSeed(seed);

  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);

}

int main() {
  setup();

  uint16_t color = genNeonColor();

  tft.drawLine(0, 30, 480, 30, color);

  // Its the map, its the map, its the map, its the map...
  Map* itsTheMap = buildDemoMap();
  itsTheMap->draw(tft, color);

  delete itsTheMap;

  Serial.end();

  return 0;
}
