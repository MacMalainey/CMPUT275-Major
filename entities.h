#pragma once

#include <MCUFRIEND_kbv.h>

#include "stdint.h"

class Drawable {
 public:
  void Move(uint16_t new_x, uint16_t new_y) {
    offset_x = new_x - start_x;
    offset_y = new_y - start_y;
  }
  void Reset() {
    offset_x = 0;
    offset_y = 0;
  }

  void Draw(MCUFRIEND_kbv& canvas) {
    for (auto i = -6; i < 7; i++) {
      for (auto j = -6; j < 7; j++) {
        if (sprite[i][j]) {
          canvas.drawPixel(start_x + offset_x, start_y + offset_y, color);
        }
      }
    }
  }

  virtual void ToggleVisibility();

  bool sprite[17][17];  // Center of sprite is x, y in global coordinate
  bool visible;

  uint16_t start_x;
  uint16_t start_y;

  int16_t offset_x;
  int16_t offset_y;

  uint16_t color;
};

class Pellet : Drawable {
 public:
  void Move(uint16_t new_x, uint16_t new_y) = delete;
};