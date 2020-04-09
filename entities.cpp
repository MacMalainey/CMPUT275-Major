/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include "include/entities.h"

Drawable::Drawable() : Drawable(Point{.x = 0, .y = 0}) {}

Drawable::Drawable(Point startPoint) : location(startPoint) {}

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

void PlayerCharacter::Move(Screen &screen, uint8_t speed) {
  Clear(screen);
  switch (orientation) {
    case NORTH:
      location.y -= speed;
      break;
    case SOUTH:
      location.y += speed;
      break;
    case EAST:
      location.x += speed;
      break;
    case WEST:
      location.x -= speed;
      break;
  }
  Draw(screen);
}

void PlayerCharacter::handleMovement(Screen &screen, uint8_t input, Map *map) {
  if (map->getXY(currentJunction) == location && input != N_ORIENT) {
    SetOrientation(input);
    Move(screen, 1);
  }

  MoveTunnel(input, screen, map);
}

bool PlayerCharacter::isValidDirection(Orientation direction) {
  if (currentJunction->next(direction) != nullptr) {
    return true;
  } else {
    return false;
  }
}

void PlayerCharacter::SetOrientation(uint8_t newOrientation) {
  if (isValidDirection(newOrientation)) {
    orientation = newOrientation;
  } else {
    orientation = N_ORIENT;
  }
}

void PlayerCharacter::checkTunnelDirection(Screen &screen, uint8_t input,
                                           Map *map, Orientation direction,
                                           Orientation opposite) {
  if (orientation == direction) {
    // If we're in a corridor, we can change directions
    if (input == opposite) {  // Switch directions
      orientation = opposite;
    } else if (input != N_ORIENT) {  // Queue a future movement
      nextDirection = input;
    }

    // Check if we're at on of two possible junctions in a corridor
    if (map->getXY(currentJunction) == location) {
      if (nextDirection != N_ORIENT) {
        SetOrientation(nextDirection);
        nextDirection = N_ORIENT;
      } else {
        SetOrientation(orientation);
      }
    } else if (map->getXY(currentJunction->next(orientation)) == location) {
      currentJunction = currentJunction->next(direction);

      if (nextDirection != N_ORIENT) {
        SetOrientation(nextDirection);
        nextDirection = N_ORIENT;
      } else {
        SetOrientation(orientation);
      }
    }

    Move(screen, 1);
  }
}

void PlayerCharacter::MoveTunnel(uint8_t input, Screen &screen, Map *map) {
  checkTunnelDirection(screen, input, map, NORTH, SOUTH);
  checkTunnelDirection(screen, input, map, SOUTH, NORTH);
  checkTunnelDirection(screen, input, map, EAST, WEST);
  checkTunnelDirection(screen, input, map, WEST, EAST);
}

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
