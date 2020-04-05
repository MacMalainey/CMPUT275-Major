#include "include/entities.h"
#include "include/grid.h"

Drawable::Drawable(uint16_t start_x, uint16_t start_y)
    : x(start_x), y(start_y) {}

void Drawable::Move(uint16_t new_x, uint16_t new_y) {
  x += new_x;
  y += new_y;
}

bool Drawable::operator!=(const Drawable &other) const { 
  return (this->x != other.x || this->y != other.y);
}

void Pellet::Draw(Screen &screen) {
  if (isVisible) {
    screen.fillCircle(x, y, 1, TFT_WHITE);
  }
}

void PlayerCharacter::Draw(Screen &screen) {
  if (isPacman) {
    screen.fillCircle(x, y, 4, TFT_YELLOW);
    screen.fillTriangle(x, y, x + 4, y + 2, x + 4, y - 2, TFT_BLACK);
  } else {
    screen.fillCircle(x, y, 7, TFT_RED);
  }
}
