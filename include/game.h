/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#pragma once

#include "comm.h"
#include "entities.h"
#include "grid.h"
#include "input.h"
#include "map.h"
#include "screen.h"
#include "vector.h"

enum State {
  SETUP,
  WAIT_FOR_SERVER,
  WAIT_FOR_CLIENT,
  READY,
};

class Game {
 public:
  Game(bool isServer);
  void Start();
  void Loop();
  bool is_running = true;

 private:
  uint8_t current_lives = 3;
  uint16_t score = 0;

  Junction *currentJunction;

  uint8_t currentDirection = 0;
  uint8_t nextDirection = 0;

  void updateScore();
  void drawLives();
  void decrementLives();
  void testGrid();
  bool isValidDirection(Orientation direction);
  void moveInTunnel(Orientation direction, uint8_t opposite);

  Map *map;
  uint16_t map_color;

  Grid grid;

  Screen screen;
  Joystick joy;

  PlayerCharacter pacman;
  PlayerCharacter ghost;

  uint16_t num_pellets = 0;
  Pellet pellets[100];

  // characters[0] should be current player.
  Vector<PlayerCharacter> characters;
  bool isServer = false;

  State GameState;
  Point startingPoint;
};
