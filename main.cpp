/**
 *
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 *
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

void drawScore(String score) {
  tft.fillRect(90, 0, DISPLAY_WIDTH, 30, TFT_BLACK);
  tft.setTextSize(2); tft.setCursor(90,6); tft.print(score);
}

void drawLives() {
  tft.fillCircle(400, 15, 8, TFT_YELLOW);
  tft.fillTriangle(400, 15, 408, 19, 408, 11, TFT_BLACK);

  tft.fillCircle(430, 15, 8, TFT_YELLOW);
  tft.fillTriangle(430, 15, 438, 19, 438, 11, TFT_BLACK);

  tft.fillCircle(460, 15, 8, TFT_YELLOW);
  tft.fillTriangle(460, 15, 468, 19, 468, 11, TFT_BLACK);
}

void setup() {
  init();
  Serial.begin(9600);

  // tft display initialization
  uint16_t ID = tft.readID();
  tft.begin(ID);

  unsigned long seed = 0;

  // Generate a new random seed at start (gotta keep things *fresh*)
  for (int i = 0; i < RAND_SEED_LENGTH; i++) {
    seed |= (analogRead(A11) & 1) << i;
  }
  randomSeed(seed);

  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);

  tft.setTextSize(2); tft.setCursor(14,6); tft.print("Score:");
  drawScore("0");
  drawLives();
}

void decrementLives(int currentLives) {
  if (currentLives == 3) {
    tft.fillRect(390, 0, 30, 30, TFT_BLACK);
  }
  else if (currentLives == 2) {
    tft.fillRect(420, 0, 30, 30, TFT_BLACK);
  }
  else {
    tft.fillRect(450, 0, 30, 30, TFT_BLACK);
  }
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
