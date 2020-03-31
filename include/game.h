#pragma once
#include <stdint.h>
#include "misc.h"
#include "screen.h"
#include "map.h"
#include "grid.h"
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

  uint8_t currentDirection = 0;
  uint8_t nextDirection = 0;

  void updateScore();
  void drawLives();
  void decrementLives();
  void movePacman();
  void testGrid();
  Orientation translateToOrien(uint8_t direction);
  uint8_t isValidDirection(uint8_t direction);
  void moveInTunnel(uint8_t direction, uint8_t opposite);

  Map map;
  uint16_t map_color;

  Grid grid;

  Screen screen;
  Joystick joy;

  PlayerCharacter pacman;
  PlayerCharacter enemy;
};
