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

  MapBuilder mb;
  mb.TestGen();
  map = mb.Build();
  map_color = genNeonColor();

  // Spatial partitioning
  grid.Generate(10);  // 10 is the number of divisions

  for(uint8_t i = 0; i < 2; i++) {
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

void ServerGame::testGrid() {

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
  if (GameState == SETUP) return; // We need to call setup first

  // get joystick input
  uint8_t input = joy.ReadInput();

  for(uint8_t i = 0; i < 2; i++) {
    if (devices[i].getState() != DISCONNECTED) {}
        devices[i].handle();
  }

  switch(GameState) {
    case SETUP: // Make the compiler happy
      break;
    case WAIT_FOR_CONNECTION:
      break;
    case READY:
      break;
  }

  // handle movement
  myChar.handleMovement(screen, input, map);

  // handle collisions between pacman and pellets
  bool collected = grid.update(myChar);

  if (collected) {
    score += 10;
    updateScore();
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
  startingPoint = map->getXY(map->GetStart());
  myChar = PlayerCharacter(startingPoint);
  myChar.currentJunction = map->GetStart();

  for(uint8_t i = 0; i < 2; i++) {
    devices[i].begin();
  }

  GameState = WAIT_FOR_CONNECTION;

  testGrid();
}

ClientGame::ClientGame() {
  map_color = genNeonColor();
}

void ClientGame::Start() {
  device.begin();

  GameState = WAIT_FOR_CONNECTION;
}

void ClientGame::Loop() {
  if (GameState == SETUP) return; // We need to call setup first

  device.handle();

  switch(GameState) {
    case SETUP: // Make the compiler happy
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
        GameState = READY;
      }
      break;
    case READY:
      break;
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
