#pragma once

#include <MCUFRIEND_kbv.h>
#include <stdint.h>

#include "misc.h"
#include "screen.h"

class Drawable {
 public:
  Drawable(uint16_t start_x = 0, uint16_t start_y = 0);

  void Move(uint16_t new_x, uint16_t new_y);

  virtual void Draw(Screen &) = 0;

  Orientation queuedMovement;
  int immediateMovement[4];

  uint16_t x;
  uint16_t y;

  uint16_t color;

  bool isVisible = true;
};

struct Pellet : public Drawable {

  void Draw(Screen &screen) final;

};

struct PlayerCharacter : public Drawable {

  void Draw(Screen &screen) final;

  bool is_pacman = true;
};