#pragma once

#include "entities.h"
#include "grid.h"
#include "input.h"
#include "map.h"
#include "screen.h"
#include "comm.h"

class Game {
 public:
  Game(bool isServer);
  void Start();
  void Loop();
  bool is_running = true;

 private:
  int current_lives = 3;
  uint16_t score = 0;

  void updateScore();
  void drawLives();
  void decrementLives();

  void testGrid();

  Map map;
  uint16_t map_color;

  Grid grid;

  Screen screen;
  Joystick joy;

  PlayerCharacter pacman;
  PlayerCharacter enemy;
};