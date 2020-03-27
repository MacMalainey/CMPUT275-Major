#pragma once

#include "stdint.h"

class Drawable {
 public:
  virtual void Update();
  void Move(uint16_t new_x, uint16_t new_y);
  virtual void Reset() = 0;

  uint16_t x;
  uint16_t y;
};