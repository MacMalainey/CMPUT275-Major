#include "include/entities.h"

Drawable::Drawable() : Drawable(Point{.x = 0, .y = 0}) {}

Drawable::Drawable(Point startPoint) : location(startPoint) {}

void Drawable::Move(Screen &screen) {
  Clear(screen);
  switch (orientation) {
    case NORTH:
      location.y--;
      break;
    case SOUTH:
      location.y++;
      break;
    case EAST:
      location.x++;
      break;
    case WEST:
      location.x--;
      break;
  }
  Draw(screen);
}

void Drawable::SetOrientation(Orientation &newOrientation) {
  orientation = newOrientation;
}

bool Drawable::operator!=(const Drawable &other) const {
  return (this->location.x != other.location.x ||
          this->location.y != other.location.y);
}

Pellet::Pellet() : Drawable() {}
Pellet::Pellet(Point startPoint) : Drawable(startPoint) {}

void Pellet::Draw(Screen &screen) {
  if (isPowerUp) {
    screen.fillCircle(location.x, location.y, 2, TFT_WHITE);
  } else {
    screen.fillCircle(location.x, location.y, 1, TFT_WHITE);
  }
}

void Pellet::Clear(Screen &screen) {
  screen.fillCircle(location.x, location.y, 2, TFT_BLACK);
}

PlayerCharacter::PlayerCharacter() : Drawable() {}
PlayerCharacter::PlayerCharacter(Point startPoint) : Drawable(startPoint) {}

void PlayerCharacter::Draw(Screen &screen) {
  if (isPacman) {
    screen.fillCircle(location.x, location.y, 4, TFT_YELLOW);
    switch (orientation) {
      case NORTH:
        screen.fillTriangle(location.x, location.y, location.x + 2,
                            location.y - 4, location.x - 2, location.y - 4,
                            TFT_BLACK);
        break;
      case SOUTH:
        screen.fillTriangle(location.x, location.y, location.x + 2,
                            location.y + 4, location.x - 2, location.y + 4,
                            TFT_BLACK);
        break;
      case EAST:
        screen.fillTriangle(location.x, location.y, location.x + 4,
                            location.y + 2, location.x + 4, location.y - 2,
                            TFT_BLACK);
        break;
      case WEST:
        screen.fillTriangle(location.x, location.y, location.x - 4,
                            location.y + 2, location.x - 4, location.y - 2,
                            TFT_BLACK);
        break;
    }
  } else {
    DrawGhostBody(screen);
    switch (orientation) {
      case NORTH:

        screen.fillRect(location.x - 2, location.y - 1, 1, 1, TFT_BLACK);
        screen.fillRect(location.x - 2, location.y - 2, 1, 1, TFT_WHITE);

        screen.fillRect(location.x + 1, location.y - 1, 1, 1, TFT_BLACK);
        screen.fillRect(location.x + 1, location.y - 2, 1, 1, TFT_WHITE);
        break;
      case SOUTH:

        screen.fillRect(location.x - 2, location.y - 1, 1, 1, TFT_BLACK);
        screen.fillRect(location.x - 2, location.y - 2, 1, 1, TFT_WHITE);

        screen.fillRect(location.x + 1, location.y - 1, 1, 1, TFT_BLACK);
        screen.fillRect(location.x + 1, location.y - 2, 1, 1, TFT_WHITE);
        break;
      case EAST:

        screen.fillRect(location.x - 1, location.y - 1, 1, 1, TFT_BLACK);
        screen.fillRect(location.x - 1, location.y - 2, 1, 1, TFT_WHITE);

        screen.fillRect(location.x + 2, location.y - 1, 1, 1, TFT_BLACK);
        screen.fillRect(location.x + 2, location.y - 2, 1, 1, TFT_WHITE);
        break;
      case WEST:

        screen.fillRect(location.x - 3, location.y - 1, 1, 1, TFT_BLACK);
        screen.fillRect(location.x - 3, location.y - 2, 1, 1, TFT_WHITE);

        screen.fillRect(location.x, location.y - 1, 1, 1, TFT_BLACK);
        screen.fillRect(location.x, location.y - 2, 1, 1, TFT_WHITE);
        break;
    }
  }
}
void PlayerCharacter::DrawGhostBody(Screen &screen) {
  screen.fillRect(location.x - 3, location.y + 3, 6, 6, TFT_RED);
  screen.fillRect(location.x - 4, location.y + 1, 1, 5, TFT_RED);
  screen.fillRect(location.x + 3, location.y + 1, 1, 5, TFT_RED);
  screen.fillRect(location.x - 2, location.y + 4, 4, 1, TFT_RED);
  screen.fillRect(location.x - 2, location.y - 3, 1, 1, TFT_RED);
  screen.fillRect(location.x + 1, location.y - 3, 1, 1, TFT_RED);
}
void PlayerCharacter::Clear(Screen &screen) {
  if (isPacman) {
    screen.fillCircle(location.x, location.y, 4, TFT_BLACK);
  } else {
    screen.fillRect(location.x - 4, location.y + 4, 8, 8, TFT_BLACK);
  }
}
