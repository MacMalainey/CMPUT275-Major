/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include "include/entities.h"

/**
 * @brief Construct a new Drawable:: Drawable object
 *
 */
Drawable::Drawable() : Drawable(Point{.x = 0, .y = 0}) {}
Drawable::Drawable(Point startPoint) : location(startPoint) {}

/**
 * @brief Inequality operator for drawables. Pellets are guaranteed not to be
 *        in the same position.
 *
 * @param other The other drawable we're comparing to.
 * @return true If not equal.
 * @return false If equal.
 */
bool Drawable::operator!=(const Drawable &other) const {
  return (this->location.x != other.location.x ||
          this->location.y != other.location.y);
}

/**
 * @brief Construct a new Pellet:: Pellet object
 *
 */
Pellet::Pellet() : Drawable() {}
Pellet::Pellet(Point startPoint) : Drawable(startPoint) {}

/**
 * @brief Draws the pellet.
 *
 * @param screen The screen (drawing, etc.).
 */
void Pellet::Draw(Screen &screen) {
  if (isPowerUp) {
    screen.fillCircle(location.x, location.y, 2, TFT_WHITE);
  } else {
    screen.fillCircle(location.x, location.y, 1, TFT_WHITE);
  }
}

/**
 * @brief Removes the drawing of the player.
 *
 * @param screen The screen (drawing, etc.).
 */
void Pellet::Clear(Screen &screen) {
  screen.fillCircle(location.x, location.y, 2, TFT_BLACK);
}

/**
 * @brief Construct a new Player Character:: Player Character object
 *
 */
PlayerCharacter::PlayerCharacter() : Drawable() {}
PlayerCharacter::PlayerCharacter(Point startPoint) : Drawable(startPoint) {}

/**
 * @brief Moves the player by `speed` pixels in it's current orientation
 *        and redraws the character.
 *
 * @param screen The screen (drawing, etc.).
 * @param speed How quickly to move the player (defaults to defaultSpeed).
 */
void PlayerCharacter::Move(Screen &screen, uint8_t speed = 1) {
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

/**
 * @brief Run each game loop. Handles everything movement related.
 *
 * @param screen The screen (drawing, etc.).
 * @param input The direction of input.
 * @param map The map (junctions, etc.).
 */
void PlayerCharacter::handleMovement(Screen &screen, uint8_t input, Map *map) {
  // Handle movement if player is at a junction.
  if (map->getXY(currentJunction) == location && input != N_ORIENT) {
    SetOrientation(input);
    Move(screen);
  }
  // Handle movement if player is in a corridor.
  else {
    MoveTunnel(input, screen, map);
  }
}

/**
 * @brief Checks if the player can move in the requested direction.
 *
 * @param direction The direction we're checking.
 * @return true If we can move in the given direction.
 * @return false If we can't move in the given direction.
 */
bool PlayerCharacter::isValidDirection(Orientation direction) {
  if (currentJunction->next(direction) != nullptr) {
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Attempts to set the orientation of the player.
 *
 * @param newOrientation The direction the player is trying to move.
 */
void PlayerCharacter::SetOrientation(uint8_t newOrientation) {
  if (isValidDirection(newOrientation)) {
    orientation = newOrientation;
  } else {
    orientation = N_ORIENT;
  }
}

/**
 * @brief Handles player movement when not in a tunnel.
 *
 * @param screen The screen (drawing, etc.).
 * @param input The direction of input.
 * @param map The map (junctions, etc.).
 * @param direction The current direction we're checking in.
 * @param opposite The opposite direction to the current direction.
 */
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

    Move(screen);
  }
}

/**
 * @brief Handles player movement in each of the four possible cardinal
 *        directions.
 *
 * @param input The direction of input.
 * @param screen The screen (drawing, etc.).
 * @param map he map (junctions, etc.).
 */
void PlayerCharacter::MoveTunnel(uint8_t input, Screen &screen, Map *map) {
  checkTunnelDirection(screen, input, map, NORTH, SOUTH);
  checkTunnelDirection(screen, input, map, SOUTH, NORTH);
  checkTunnelDirection(screen, input, map, EAST, WEST);
  checkTunnelDirection(screen, input, map, WEST, EAST);
}

/**
 * @brief Draws the player. If the player is PacMan, it draws the correct
 *        sprite; otherwise it draws a ghost.
 *
 * @param screen The screen (drawing, etc.).
 */
void PlayerCharacter::Draw(Screen &screen) {
  if (isPacman) {
    screen.fillCircle(location.x, location.y, 4, color);

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

/**
 * @brief Draws the ghost sprite.
 *
 * @param screen The screen (drawing, etc.).
 */
void PlayerCharacter::DrawGhostBody(Screen &screen) {
  screen.fillRect(location.x - 3, location.y + 3, 6, 6, TFT_RED);
  screen.fillRect(location.x - 4, location.y + 1, 1, 5, TFT_RED);
  screen.fillRect(location.x + 3, location.y + 1, 1, 5, TFT_RED);
  screen.fillRect(location.x - 2, location.y + 4, 4, 1, TFT_RED);
  screen.fillRect(location.x - 2, location.y - 3, 1, 1, TFT_RED);
  screen.fillRect(location.x + 1, location.y - 3, 1, 1, TFT_RED);
}

/**
 * @brief Clears the character from the screen by drawing a black circle over
 *        the location.
 *
 * @param screen The screen (drawing, etc.).
 */
void PlayerCharacter::Clear(Screen &screen) {
  if (isPacman) {
    screen.fillCircle(location.x, location.y, 4, TFT_BLACK);
  } else {
    screen.fillRect(location.x - 4, location.y - 4, 8, 8, TFT_BLACK);
  }
}

void Pellet::GeneratePellets(Pellet *pellets, Junction *startJunction,
                             uint8_t junctionCount) {
  bool touched[junctionCount] = {false};
  Queue<Junction *> events;
  events.push(startJunction);

  touched[0] = true;

  while (events.size() > 0) {
    Junction *junct = events.pop();
    for (uint8_t orient = 0; orient < N_ORIENT; orient++) {
      if (junct->next((Orientation)orient) != nullptr) {
        Junction *adj = junct->adjacent[orient];
        if (!touched[adj->id]) {
          touched[adj->id] = true;
          events.push(adj);
        }

        auto pelletLocation = Point{.x = junct->x, .y = junct->y};
        pellets[junct->id] = Pellet(pelletLocation);
      }
    }
  }
}