#pragma once
#include "comm.h"
#include "entities.h"
#include "grid.h"
#include "input.h"
#include "map.h"
#include "screen.h"

class Game {
 public:
  Game(bool isServer);
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
  uint8_t validDirections = 0;

  void updateScore();
  void drawLives();
  void decrementLives();
  void movePacman();
  void testGrid();

  Map map;
  uint16_t map_color;

  Grid grid;

  Screen screen;
  Joystick joy;

  PlayerCharacter pacman;
  PlayerCharacter enemy;
};
