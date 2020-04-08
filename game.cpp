#include "include/game.h"

#include "include/input.h"

Game::Game(bool isServer) {
  // Setup functions basically go here
  // Generate Map
  // Determine player and enemies
  screen.setCursor(14, 6);
  screen.print("Score:");
  updateScore();
  drawLives();

  // Its the map, its the map, its the map, its the map...
  MapBuilder mb;
  mb.TestGen();
  map = mb.Build();
  map_color = genNeonColor();
  screen.drawLine(0, 30, 480, 30, map_color);

  // Spatial partitioning
  grid.Generate(10);  // 10 is the number of divisions
}

void Game::updateScore() {
  screen.fillRect(90, 0, screen.DISPLAY_WIDTH, 30, TFT_BLACK);
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
      newPellet.x = i * screen.DISPLAY_WIDTH / 10 + 15;
      newPellet.y = j * screen.DISPLAY_HEIGHT / 10 + 15;

      // newPellet.Draw(screen);

      grid.addPellet(newPellet);

      pellets[k++] = newPellet;
    }
  }

  for (uint16_t i = 0; i < num_pellets; i++) {
    grid.removePellet(pellets[i]);
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

void Game::movePacman() {
  if (currentDirection == 1u) {
    screen.fillCircle(current_x, current_y, 4, TFT_BLACK);
    current_y -= 1;
    screen.fillCircle(current_x, current_y, 4, TFT_YELLOW);
    screen.fillTriangle(current_x, current_y, current_x + 2, current_y - 4,
                        current_x - 2, current_y - 4, TFT_BLACK);
  }

  else if (currentDirection == 2u) {
    screen.fillCircle(current_x, current_y, 4, TFT_BLACK);
    current_y += 1;
    screen.fillCircle(current_x, current_y, 4, TFT_YELLOW);
    screen.fillTriangle(current_x, current_y, current_x + 2, current_y + 4,
                        current_x - 2, current_y + 4, TFT_BLACK);
  }

  else if (currentDirection == 4u) {
    screen.fillCircle(current_x, current_y, 4, TFT_BLACK);
    current_x += 1;
    screen.fillCircle(current_x, current_y, 4, TFT_YELLOW);
    screen.fillTriangle(current_x, current_y, current_x + 4, current_y + 2,
                        current_x + 4, current_y - 2, TFT_BLACK);
  }

  else if (currentDirection == 8u) {
    screen.fillCircle(current_x, current_y, 4, TFT_BLACK);
    current_x -= 1;
    screen.fillCircle(current_x, current_y, 4, TFT_YELLOW);
    screen.fillTriangle(current_x, current_y, current_x - 4, current_y + 2,
                        current_x - 4, current_y - 2, TFT_BLACK);
  }
}

// up = 0, down = 1, right = 2, left = 3
Orientation Game::translateToOrien(uint8_t direction) {
  if (direction == 1u) {
    return (Orientation)0;
  }

  else if (direction == 2u) {
    return (Orientation)1;
  }

  else if (direction == 4u) {
    return (Orientation)2;
  }

  else {  // if (direction == 8u) {
    return (Orientation)3;
  }
}

uint8_t Game::isValidDirection(uint8_t direction) {
  if (currentJunction->next((Orientation)translateToOrien(direction)) !=
      nullptr) {
    return direction;
  } else {
    return 0;
  }
}

void Game::moveInTunnel(uint8_t direction, uint8_t opposite) {
  if (currentDirection == direction) {
    uint8_t input = joy.ReadInput();

    if (input == opposite) {
      currentDirection = opposite;
    }

    else if (input != 0) {
      nextDirection = input;
    }

    if (map->getXY(currentJunction).x == current_x &&
        map->getXY(currentJunction).y == current_y) {
      if (nextDirection != 0) {
        currentDirection = isValidDirection(nextDirection);
        nextDirection = 0;
      }

      else {
        currentDirection = isValidDirection(currentDirection);
      }
    }

    else if (map->getXY(
                    currentJunction->next(translateToOrien(currentDirection)))
                     .x == current_x &&
             map->getXY(
                    currentJunction->next(translateToOrien(currentDirection)))
                     .y == current_y) {
      // Serial.print("You are at a new junction!");
      currentJunction = currentJunction->next(translateToOrien(direction));

      if (nextDirection != 0) {
        currentDirection = isValidDirection(nextDirection);
        nextDirection = 0;
      }

      else {
        currentDirection = isValidDirection(currentDirection);
      }
    }

    movePacman();
  }
}

void Game::Loop() {
  uint8_t buttonPressed = joy.ReadInput();

  if (map->getXY(currentJunction).x == current_x &&
      map->getXY(currentJunction).y == current_y && buttonPressed != 0) {
    currentDirection = isValidDirection(buttonPressed);
    movePacman();
  }

  moveInTunnel(1u, 2u);
  moveInTunnel(2u, 1u);
  moveInTunnel(4u, 8u);
  moveInTunnel(8u, 4u);

  delay(20);
}

void Game::Start() {
  screen.DrawMap(map, map_color);
  Point startingPoint = map->getXY(map->GetStart());
  current_x = startingPoint.x;
  current_y = startingPoint.y;
  pacman.Move(current_x, current_y);

  currentJunction = map->GetStart();
  pacman.Draw(screen);

  enemy.isPacman = false;

  enemy.Move(150, 200);
  enemy.Draw(screen);

  testGrid();
}
