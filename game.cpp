#include "include/game.h"

Game::Game() {
    // Setup functions basically go here

    // Generate Map
    // Determine player and enemies
    screen.setCursor(14, 6);
    screen.print("Score:");
    updateScore();
    drawLives();

    // Its the map, its the map, its the map, its the map...
    map.Generate();
    map_color = genNeonColor();
    screen.drawLine(0, 30, 480, 30, map_color);

    // Spatial partitioning
    grid.Generate(10);
    testGrid();
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
    screen.fillCircle(current_x, current_y, 4, TFT_BLACK);
    if (currentDirection == 1000) {
      current_y -= 1;
    }
    else if (currentDirection == 0100) {
      current_y += 1;
    }
    else if (currentDirection == 0010) {
      current_x += 1;
    }
    else if (currentDirection == 0001) {
      current_x -= 1;
    }
    screen.fillCircle(current_x, current_y, 4, TFT_YELLOW);
    screen.fillTriangle(current_x, current_y, current_x + 4, current_y + 2, current_x + 4, current_y - 2, TFT_BLACK);
}

void Game::Loop() {
    uint8_t buttonPressed = ReadInput();

    if (buttonPressed == 1000) {
      if (currentJunction.next(0) != nullptr) {
        currentDirection = 1000;
      }
    }
    else if (buttonPressed == 0100) {
      if (currentJunction.next(1) != nullptr) {
        currentDirection = 0100;
      }
    }
    else if (buttonPressed == 0010) {
      if (currentJunction.next(2) != nullptr) {
        currentDirection = 0010;
      }
    }
    else if (buttonPressed == 0001) {
      if (currentJunction.next(3) != nullptr) {
        currentDirection = 0001;
      }
    }

}

void Game::Start() {
    screen.DrawMap(map, map_color);

    Point startingPoint = map.getSpawnXY();
    current_x = startingPoint.x;
    current_y = startingPoint.y;
    pacman.Move(current_x, current_y);

    currentJunction = map.GetStart();
    pacman.Draw(screen);


    enemy.is_pacman = false;

    enemy.Move(150, 200);
    enemy.Draw(screen);
}
