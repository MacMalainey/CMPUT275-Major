#pragma once
#include <stdint.h>
#include "misc.h"
#include "screen.h"
#include "map.h"
#include "grid.h"
#include "assets.h"
#include "entities.h"
#include "input.h"

class Game {

 public:
  Game();
  void Start();
  void Loop();
  bool is_running = true;

 private:

  int current_lives = 3;
  uint16_t score = 0;
  int current_x;
  int current_y;

  Junction *currentJunction;

  uint8_t currentDirection;

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
