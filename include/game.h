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
  WAIT_FOR_CONNECTION,
  READY
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
  void testGrid();

  uint16_t distUntilDeadEnd(Point gLocation, Junction *junction, 
                            Orientation orientation);
  void canSeePacman(PlayerCharacter ghost);
  
  Map *map;
  uint16_t map_color;

  Grid grid;

  Screen screen;
  Joystick joy;

  PlayerCharacter myChar;
  PlayerCharacter ghost;

  uint16_t num_pellets = 0;
  Pellet pellets[100];

  // characters[0] should be PacMan.
  Vector<PlayerCharacter> characters;

  Server* devices[3];
  State gameState;
  Point startingPoint;

};

class ClientGame {
public:

  ClientGame();

  void Start();
  void Loop();

private:
  State gameState;
  Client* device;

  Screen screen;
  Joystick joy;

  uint16_t map_color;

  Map *map;

  Vector<PlayerCharacter> characters;

  PlayerCharacter myChar;

  uint16_t score = 0;
  uint8_t current_lives = 3;

  void updateScore();
  void decrementLives();
  void drawLives();

};