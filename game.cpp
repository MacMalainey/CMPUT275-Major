/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include "include/game.h"

Game::Game() : characters(3) {
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
  Pellet::GeneratePellets(pellets, map);

  for (uint16_t i = 0; i < pellets.Size(); i++) {
    pellets.Get(i).Draw(screen);
  }
}
