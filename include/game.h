#pragma once

#include "screen.h"
#include "map.h"
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

  void updateScore();
  void drawLives();
  void decrementLives();

  Map map;
  uint16_t map_color;

  Screen screen;
  Joystick joy;

  PlayerCharacter pacman;
  PlayerCharacter enemy;
};