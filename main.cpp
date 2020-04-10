/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include <Arduino.h>

#include "include/game.h"
#include "include/map.h"
#include "include/misc.h"
#include "include/multi.h"
#include "include/screen.h"

#define RAND_SEED_LENGTH 32
#define RAND_READ_PIN    A11

#define SERVER_SELECT_PIN 53

void setup() {
  init();
  Serial.begin(9600);

  uint32_t seed = 0;

  //  Generate a new random seed at start (gotta keep things *fresh*)
  for (int i = 0; i < RAND_SEED_LENGTH; i++) {
    seed |= (analogRead(RAND_READ_PIN) & 1) << i;
  }

  randomSeed(seed);
}

int main() {
  setup();

  if (digitalRead(SERVER_SELECT_PIN) == HIGH) {
    ServerGame* game = new ServerGame();
    game->Start();

    while (true) {
      game->Loop();
    }
  } else {
    ClientGame* game = new ClientGame();
    game->Start();

    while (true) {
      game->Loop();
    }
  }

  Serial.end();
}
