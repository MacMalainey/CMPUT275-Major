/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include "include/game.h"

Game::Game(bool isServer) : isServer(isServer), characters(3) {
  // Draw UI
  screen.setCursor(14, 6);
  screen.print("Score:");
  updateScore();
  drawLives();

  MapBuilder mb;
  mb.TestGen();
  map = mb.Build();
  map_color = genNeonColor();
  screen.drawLine(0, 30, 480, 30, map_color);

  // Spatial partitioning
  grid.Generate(10);  // 10 is the number of divisions

  GameState = SETUP;

  // if (isServer) {
  //   GameState = WAIT_FOR_CLIENT;
  // } else {
  //   GameState = WAIT_FOR_SERVER;
  // }
}

void Game::updateScore() {
  screen.fillRect(90, 6, screen.DISPLAY_WIDTH / 4, 18, TFT_BLACK);
  screen.setCursor(90, 6);
  screen.print(score);
}

void Game::decrementLives() {
  if (current_lives == 3) {
    screen.fillRect(390, 0, 30, 30, TFT_BLACK);
  } else if (current_lives == 2) {
    screen.fillRect(420, 0, 30, 30, TFT_BLACK);
  } else {
    screen.fillRect(450, 0, 30, 30, TFT_BLACK);
  }
}

void Game::testGrid() {
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

  num_pellets = 100;
  uint16_t k = 0;

  for (uint16_t i = 0; i < num_pellets / 10; i++) {
    for (uint16_t j = 0; j < num_pellets / 10; j++) {
      Pellet newPellet;
      newPellet.location.x = i * Screen::DISPLAY_WIDTH / 10 + 25;
      newPellet.location.y = j * Screen::DISPLAY_HEIGHT / 10 + 15;

      newPellet.Draw(screen);

      grid.addPellet(newPellet);

      pellets[k++] = newPellet;
    }
  }

  // for (uint16_t i = 0; i < num_pellets; i++) {
  //   grid.removePellet(pellets[i]);
  // }
}

void Game::drawLives() {
  screen.fillCircle(400, 15, 8, TFT_YELLOW);
  screen.fillTriangle(400, 15, 408, 19, 408, 11, TFT_BLACK);

  screen.fillCircle(430, 15, 8, TFT_YELLOW);
  screen.fillTriangle(430, 15, 438, 19, 438, 11, TFT_BLACK);

  screen.fillCircle(460, 15, 8, TFT_YELLOW);
  screen.fillTriangle(460, 15, 468, 19, 468, 11, TFT_BLACK);
}

void Game::Loop() {
  // get joystick input
  uint8_t input = joy.ReadInput();

  // handle movement
  pacman.handleMovement(screen, input, map);

  // handle collisions between pacman and pellets
  bool collected = grid.update(pacman);

  if (collected) {
    score += 10;
    updateScore();
  }

  delay(20);
}

void Game::Start() {
  GameState = SETUP;
  
  while (GameState != READY) {
    switch (GameState) {
      case WAIT_FOR_SERVER:
        // We client
        break;
      case WAIT_FOR_CLIENT:
        // We server
        break;
      case SETUP:
        screen.DrawMap(map, map_color);
        startingPoint = map->getXY(map->GetStart());
        pacman = PlayerCharacter(startingPoint);
        pacman.currentJunction = map->GetStart();

        GameState = READY;
        break;
    }
  }
  
  ghost.isPacman = false;

  ghost.Move(screen);

  testGrid();
}
