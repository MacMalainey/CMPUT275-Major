/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include "include/game.h"

ServerGame::ServerGame() {
  MapBuilder mb;
  mb.TestGen();
  map = mb.Build();
  map_color = genNeonColor();

  // Spatial partitioning
  grid.Generate(10);  // 10 is the number of divisions
  for (uint8_t i = 0; i < 2; i++) {
    devices[i] = Server(i);
    devices[i].mCallback.Debuild(map);
  }

  GameState = SETUP;
}

void ServerGame::updateScore() {
  screen.fillRect(90, 6, screen.DISPLAY_WIDTH / 4, 18, TFT_BLACK);
  screen.setCursor(90, 6);
  screen.print(score);
}

void ServerGame::decrementLives() {
  if (current_lives == 3) {
    screen.fillRect(390, 0, 30, 30, TFT_BLACK);
  } else if (current_lives == 2) {
    screen.fillRect(420, 0, 30, 30, TFT_BLACK);
  } else {
    screen.fillRect(450, 0, 30, 30, TFT_BLACK);
  }
}

Orientation reverseOrien(Orientation orientation) {
  if (orientation == N_ORIENT) {
    return N_ORIENT;
  }
  if (orientation == NORTH) {
    return SOUTH;
  }
  if (orientation == SOUTH) {
    return NORTH;
  }
  if (orientation == EAST) {
    return WEST;
  }
  return EAST;
}

uint16_t ClientGame::distUntilDeadEnd(Point gLocation, Junction *junction,
                                      Orientation orientation) {
  Junction *current = junction;

  // get the farthest junction in current direction
  while (current->next(orientation) != nullptr) {
    current = current->next(orientation);
  }

  return (map->getXY(current) - gLocation);
}

void ClientGame::canSeePacman(PlayerCharacter ghost) {
  Point pLocation = characters[0].location;
  Point gLocation = ghost.location;

  if (pLocation.x == gLocation.x || pLocation.y == gLocation.y) {
    // We now need to check there is a straight line.

    uint16_t distPac = pLocation - gLocation;  // Manhattan distance
    Orientation orien = ghost.orientation;
    Junction *currentJunc = ghost.currentJunction;

    // Ghost is at a junction, check all valid junction directions.
    if (map->getXY(currentJunc) == gLocation) {
      for (uint8_t i = 0; i < N_ORIENT; i++) {
        if (currentJunc->next((Orientation)i) != nullptr) {
          uint16_t dist =
              distUntilDeadEnd(gLocation, currentJunc, (Orientation)i);

          if (dist >= distPac) {
            ghost.canSeePacman = true;
            Serial.println("Visible1");
            return;
          }
        }
      }

      ghost.canSeePacman = false;
    }

    // Ghost is in a corridor, check orientation and opposite orientation.
    else {
      uint16_t dist = distUntilDeadEnd(gLocation, currentJunc, orien);
      uint16_t distR =
          distUntilDeadEnd(gLocation, currentJunc, reverseOrien(orien));

      if (dist >= distPac || distR >= distPac) {
        ghost.canSeePacman = true;
        Serial.println("Visible2");
        return;
      } else {
        ghost.canSeePacman = false;
      }
    }
  }
}

void ServerGame::drawLives() {
  screen.fillCircle(400, 15, 8, TFT_YELLOW);
  screen.fillTriangle(400, 15, 408, 19, 408, 11, TFT_BLACK);

  screen.fillCircle(430, 15, 8, TFT_YELLOW);
  screen.fillTriangle(430, 15, 438, 19, 438, 11, TFT_BLACK);

  screen.fillCircle(460, 15, 8, TFT_YELLOW);
  screen.fillTriangle(460, 15, 468, 19, 468, 11, TFT_BLACK);
}

void ServerGame::Loop() {
  if (GameState == SETUP) return;  // We need to call setup first

  // get joystick input
  uint8_t input = joy.ReadInput();

  for (uint8_t i = 0; i < 2; i++) {
    if (devices[i].getState() != DISCONNECTED) devices[i].handle();
  }

  switch (GameState) {
    case SETUP:  // Make the compiler happy
      break;
    case WAIT_FOR_CONNECTION: {
      // Assume true
      bool allCon = true;

      // If true then for each the following should be true
      for (uint8_t i = 0; i < 2; i++) {
        ComState ds = devices[i].getState();

        // If true then that means our assumption is false
        if (ds != LOOP && ds != DISCONNECTED) allCon = false;
      }

      // And that my friends is how you do a proof by contradiction
      // Technically it is by exhaustion but that ruins the joke
      if (allCon) {
        GameState = READY;
      }
    } break;
    case READY:
      // handle movement
      myChar.handleMovement(screen, input, map);

      PlayerPayload p;
      p.id = 0;
      p.x = myChar.location.x;
      p.y = myChar.location.y;

      for (uint8_t i = 0; i < 2; i++) {
        if (devices[i].getState() != DISCONNECTED) {
          devices[i].sendEntityLocation(p);
          if (devices[i].pCallback.hasData) {
            devices[i].pCallback.hasData = false;
            uint8_t id = devices[i].pCallback.load.id;
            characters[id].Clear(screen);
            characters[id].location.x = devices[i].pCallback.load.x;
            characters[id].location.y = devices[i].pCallback.load.y;
            characters[id].Draw(screen);
          }
        }
      }

      // handle collisions between pacman and pellets
      bool collected = grid.update(myChar);

      if (collected) {
        score += 10;
        updateScore();
      }

      break;
  }
  delay(20);
}

void ServerGame::Start() {
  // Draw UI
  screen.drawLine(0, 30, 480, 30, map_color);
  screen.setCursor(14, 6);
  screen.print("Score:");
  updateScore();
  drawLives();

  screen.DrawMap(map, map_color);

  Pellet::GeneratePellets(pellets, map->GetStart(), map->GetNodeCount());
  for (size_t i = 0; i < 70; i++) {
    pellets[i].Draw(screen);
  }
  startingPoint = map->getXY(map->GetStart());
  myChar = PlayerCharacter(startingPoint);
  myChar.currentJunction = map->GetStart();
  myChar.color = TFT_YELLOW;

  for (uint8_t i = 0; i < 2; i++) {
    devices[i].begin();
  }

  characters[1].color = genNeonColor();
  characters[2].color = genNeonColor();

  GameState = WAIT_FOR_CONNECTION;
}

ClientGame::ClientGame() { map_color = genNeonColor(); }

void ClientGame::Start() {
  device.begin();

  GameState = WAIT_FOR_CONNECTION;
}

void ClientGame::Loop() {
  if (GameState == SETUP) return;  // We need to call setup first

  // get joystick input
  uint8_t input = joy.ReadInput();

  device.handle();

  switch (GameState) {
    case SETUP:  // Make the compiler happy
      break;
    case WAIT_FOR_CONNECTION:
      if (device.getState() == LOOP) {
        map = device.mCallback.Build();

        // Draw UI
        screen.drawLine(0, 30, 480, 30, map_color);
        screen.setCursor(14, 6);
        screen.print("Score:");
        updateScore();
        drawLives();

        screen.DrawMap(map, map_color);
        myChar = PlayerCharacter(map->getXY(map->GetStart()));
        myChar.currentJunction = map->GetStart();
        myChar.color = genNeonColor();
        characters[0].color = TFT_YELLOW;
        GameState = READY;
      }
      break;
    case READY:
      // handle movement
      myChar.handleMovement(screen, input, map);

      PlayerPayload p;
      p.id = device.getID();
      p.x = myChar.location.x;
      p.y = myChar.location.y;

      device.sendEntityLocation(p);
      if (device.pCallback.hasData) {
        device.pCallback.hasData = false;
        // TODO other player's movement
      }

      break;
  }
  delay(20);
}

void ClientGame::drawLives() {
  screen.fillCircle(400, 15, 8, TFT_YELLOW);
  screen.fillTriangle(400, 15, 408, 19, 408, 11, TFT_BLACK);

  screen.fillCircle(430, 15, 8, TFT_YELLOW);
  screen.fillTriangle(430, 15, 438, 19, 438, 11, TFT_BLACK);

  screen.fillCircle(460, 15, 8, TFT_YELLOW);
  screen.fillTriangle(460, 15, 468, 19, 468, 11, TFT_BLACK);
}

void ClientGame::decrementLives() {
  if (current_lives == 3) {
    screen.fillRect(390, 0, 30, 30, TFT_BLACK);
  } else if (current_lives == 2) {
    screen.fillRect(420, 0, 30, 30, TFT_BLACK);
  } else {
    screen.fillRect(450, 0, 30, 30, TFT_BLACK);
  }
}

void ClientGame::updateScore() {
  screen.fillRect(90, 6, screen.DISPLAY_WIDTH / 4, 18, TFT_BLACK);
  screen.setCursor(90, 6);
  screen.print(score);
}
