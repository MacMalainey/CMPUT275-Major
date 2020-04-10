/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#pragma once

#include "entities.h"
#include "grid.h"
#include "input.h"
#include "map.h"
#include "multi.h"
#include "screen.h"
#include "vector.h"
#include "multi.h"
#include "misc.h"

enum State {
  SETUP,
  WAIT_FOR_CONNECTION,
  READY,
};

class ServerGame {
 public:
  ServerGame();
  void Start();
  void Loop();

 private:
  uint8_t current_lives = 3;
  uint16_t score = 0;

  void updateScore();
  void drawLives();
  void decrementLives();

  Map *map;
  uint16_t map_color;

  Grid grid;

  Screen screen;
  Joystick joy;
  Server devices[2];

  PlayerCharacter myChar;
  PlayerCharacter ghost;

  Vector<Pellet> pellets;

  // characters[0] should be PacMan.
  PlayerCharacter characters[3];

  State GameState;
  Point startingPoint;
};

class ClientGame {
 public:
  ClientGame();
  void Start();
  void Loop();

 private:
  void updateScore();
  void drawLives();
  void decrementLives();

  uint16_t distUntilDeadEnd(Point gLocation, Junction *junction,
                            Orientation orientation);
  void canSeePacman(PlayerCharacter ghost);

  State GameState;

  Screen screen;
  Joystick joy;
  Client device;

  uint8_t current_lives = 3;
  uint16_t score = 0;

  Map *map;
  uint16_t map_color;

  PlayerCharacter myChar;

  // characters[0] should be PacMan.
  PlayerCharacter characters[3];
};
