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
  uint8_t current_lives = 3;
  uint16_t score = 0;
  uint16_t current_x;
  uint16_t current_y;

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

  uint16_t num_pellets = 0;
  Pellet pellets[100];
};
