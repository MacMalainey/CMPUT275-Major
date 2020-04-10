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

// Left some test code in here for myself
// Please don't delete -- Mac

// Map* TEST_MAP_GET() {
//   Serial.println("Map getter called");
//   Screen s;
//   MapBuilder mb;
//   mb.TestGen();
//   Map* map = mb.Build();
//   uint16_t map_color = genNeonColor();

//   s.DrawMap(map, map_color);

//   return map;
// }

// void TEST_MAP_SET(Map* map) {
//   Serial.println("Map setter called");
//   Screen s;
//   uint16_t map_color = genNeonColor();

//   s.DrawMap(map, map_color);
// }

int main() {
  setup();

  // Left some test code in here for myself
  // Please don't delete -- Mac

  // Device* d;
  // ComState state = START;

  // if (digitalRead(SERVER_SELECT_PIN) == HIGH) {
  //   Server* s = new Server(1);
  //   s->mCallback = TEST_MAP_GET;
  //   d = s;
  // } else {
  //   Client* c = new Client();
  //   c->mCallback = TEST_MAP_SET;
  //   d = c;
  // }

  // d->begin();

  // for (;;) {
  //   d->handle();
  //   if (d->getState() != state) {
  //     state = d->getState();
  //     Serial.print("Transitioned State: ");
  //     Serial.print(state);
  //   }
  // }

  if (digitalRead(SERVER_SELECT_PIN) == HIGH) {
    ServerGame game;
    Serial.println("111111111111111111111111111");
    game.Start();
    Serial.println("222222222222222222222222222");
    while (true) {
      game.Loop();
    }
  } else {
    ClientGame game;
    game.Start();

    while (true) {
      game.Loop();
    }
  }

  Serial.end();
}
