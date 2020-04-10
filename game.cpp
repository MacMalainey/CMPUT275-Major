/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include "include/game.h"

ServerGame::ServerGame() : characters(3) {
  // Draw UI
  screen.setCursor(14, 6);
  screen.print("Score:");
  updateScore();
  drawLives();

  MapBuilder mb;
  mb.TestGen();
  map = mb.Build();
  map_color = genNeonColor();

  // Spatial partitioning
  grid.Generate(10);  // 10 is the number of divisions

  for (uint8_t i = 0; i < 3; i++) {
    devices[i] = new Server(i + 1);
    devices[i]->builder = new MapBuilder();
    devices[i]->builder->Debuild(map);
    devices[i]->pCallback = new PlayerCallback();
    devices[i]->sCallback = new StateCallback();
  }
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

void ServerGame::testGrid() {
  // for (uint8_t i = 0; i < grid.divisions; i++) {
  //     for (uint8_t j = 0; j < grid.divisions; j++) {
  //         Row *row_i = grid.getRow(i);
  //         Cell *cell_ij = row_i->getCell(j);
  //         uint16_t id = cell_ij->getID();

  //         Serial.println(id);
  //     }
  // }

  // Serial.println(grid.getRowIndex(0));
  // Serial.println(grid.getRowIndex(10));
  // Serial.println(grid.getRowIndex(100));
  // Serial.println(grid.getRowIndex(200));
  // Serial.println(grid.getRowIndex(300));
  // Serial.println(grid.getRowIndex(480));

  // num_pellets = 100;
  // uint16_t k = 0;

  // for (uint16_t i = 0; i < num_pellets / 10; i++) {
  //   for (uint16_t j = 0; j < num_pellets / 10; j++) {
  //     Pellet newPellet;
  //     newPellet.location.x = i * Screen::DISPLAY_WIDTH / 10 + 25;
  //     newPellet.location.y = j * Screen::DISPLAY_HEIGHT / 10 + 15;

  //     newPellet.Draw(screen);

  //     grid.addPellet(newPellet);

  //     pellets[k++] = newPellet;
  //   }
  // }

  // for (uint16_t i = 0; i < num_pellets; i++) {
  //   grid.removePellet(pellets[i]);
  // }
}

Orientation reverseOrien(Orientation orientation) {
  if (orientation == N_ORIENT) { return N_ORIENT; }
  if (orientation == NORTH) { return SOUTH; }
  if (orientation == SOUTH ) { return NORTH; }
  if (orientation == EAST ) { return WEST; }
  return EAST; 
}

uint16_t ServerGame::distUntilDeadEnd(Point gLocation, 
Junction *junction, Orientation orientation) {

  Junction *current = junction;

  // get the farthest junction in current direction
  while (current->next(orientation) != nullptr) {
    current = current->next(orientation);
  }

  return (map->getXY(current) - gLocation);
}

void ServerGame::canSeePacman(PlayerCharacter ghost) {
  Point pLocation = characters.Get(0).location;
  Point gLocation = ghost.location;
  
  if (pLocation.x == gLocation.x || pLocation.y == gLocation.y) {
    // We now need to check there is a straight line.

    uint16_t distPac = pLocation - gLocation; // Manhattan distance
    Orientation orien = ghost.orientation;
    Junction *currentJunc = ghost.currentJunction;

    // Ghost is at a junction, check all valid junction directions.
    if (map->getXY(currentJunc) == gLocation) {
      for (uint8_t i = 0; i < N_ORIENT; i++) {
        if (currentJunc->next((Orientation)i) != nullptr) {
          uint16_t dist = distUntilDeadEnd(gLocation, currentJunc, (Orientation)i);

          if (dist >= distPac) {
            ghost.canSeePacman = true;
            return;
          }
        }
      }

      ghost.canSeePacman = false;
    } 

    // Ghost is in a corridor, check orientation and opposite orientation.
    else {
      uint16_t dist = distUntilDeadEnd(gLocation, currentJunc, orien);
      uint16_t distR = distUntilDeadEnd(gLocation, currentJunc, reverseOrien(orien));

      if (dist >= distPac || distR >= distPac) {
        ghost.canSeePacman = true;
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
  for (uint8_t i = 0; i < 3; i++) {
    devices[i]->handle();
  }

  switch (gameState) {
    case WAIT_FOR_CONNECTION:

      // Assume true
      bool allCon = true;

      // If true then for each the following should be true
      for (uint8_t i = 0; i < 3; i++) {
        ComState ds = devices[i]->getState();

        // If true then that means our assumption is false
        if (ds != LOOP && ds != DISCONNECTED) allCon = false;
      }

      // And that my friends is how you do a proof by contradiction

      // Handle state change
      if (allCon) {
        Serial.println("WE CONNECTED OK?");
      }

      break;
  }
  // get joystick input
  uint8_t input = joy.ReadInput();

  // handle movement
  myChar.handleMovement(screen, input, map);

  // handle line of sight for ghosts
  for (uint8_t i = 1; i < characters.Size(); i++) {
    canSeePacman(characters.Get(i));
  }

  // handle collisions between pacman and pellets
  bool collected = grid.update(myChar);

  if (collected) {
    score += 10;
    updateScore();
  }

  delay(20);
}
        
void ServerGame::Start() {
  Serial.println("yeet");
  screen.DrawMap(map, map_color);
  screen.drawLine(0, 30, 480, 30, map_color);
  startingPoint = map->getXY(map->GetStart());
  myChar = PlayerCharacter(startingPoint);
  myChar.currentJunction = map->GetStart();

  for (uint8_t i = 0; i < 3; i++) {
    devices[i]->begin();
  }
  Pellet::GeneratePellets(pellets, map);

  gameState = WAIT_FOR_CONNECTION;

  testGrid();
}

ClientGame::ClientGame() {
  map_color = genNeonColor();

  device = new Client();
  device->builder = new MapBuilder();
}

void ClientGame::Start() {
  device->begin();

  gameState = WAIT_FOR_CONNECTION;
}

void ClientGame::Loop() {
  device->handle();

  switch (gameState) {
    case WAIT_FOR_CONNECTION:
      if (device->getState() == LOOP) {
        map = device->builder->Build();

        // Draw UI
        screen.setCursor(14, 6);
        screen.print("Score:");
        updateScore();
        drawLives();

        screen.drawLine(0, 30, 480, 30, map_color);
        screen.DrawMap(map, map_color);

        delete device->builder;

        gameState = READY;
      }
      break;
    case READY:
      Serial.println("WE HAVE A CONNECTION");
      break;
  }
}

void ClientGame::updateScore() {
  screen.fillRect(90, 6, screen.DISPLAY_WIDTH / 4, 18, TFT_BLACK);
  screen.setCursor(90, 6);
  screen.print(score);
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

void ClientGame::drawLives() {
  screen.fillCircle(400, 15, 8, TFT_YELLOW);
  screen.fillTriangle(400, 15, 408, 19, 408, 11, TFT_BLACK);

  screen.fillCircle(430, 15, 8, TFT_YELLOW);
  screen.fillTriangle(430, 15, 438, 19, 438, 11, TFT_BLACK);

  screen.fillCircle(460, 15, 8, TFT_YELLOW);
  screen.fillTriangle(460, 15, 468, 19, 468, 11, TFT_BLACK);
}
