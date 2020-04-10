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
#include "screen.h"
#include "vector.h"
#include "multi.h"

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

  Junction *currentJunction;

  Orientation currentDirection = Orientation::EAST;
  Orientation nextDirection;

  void updateScore();
  void drawLives();
  void decrementLives();
  void testGrid();
  bool isValidDirection(Orientation direction);
  // void moveInTunnel(Orientation direction, uint8_t opposite);

  Map *map;
  uint16_t map_color;

  Grid grid;

  Screen screen;
  Joystick joy;
  Server devices[2];

  PlayerCharacter myChar;
  PlayerCharacter ghost;

  uint16_t num_pellets = 0;
  Pellet pellets[100];

  // characters[0] should be current player.
  Vector<PlayerCharacter> characters;

  State GameState;
  Point startingPoint;
};

class ClientGame {

public:
  ClientGame();
  void Start();
  void Loop();

  void updateScore();
  void drawLives();
  void decrementLives();

  State GameState;

  Screen screen;
  Joystick joy;
  Client device;

  uint8_t current_lives = 3;
  uint16_t score = 0;

  Map *map;
  uint16_t map_color;

  PlayerCharacter myChar;

};
