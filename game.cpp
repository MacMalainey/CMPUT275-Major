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

void Game::drawLives() {
    screen.fillCircle(400, 15, 8, TFT_YELLOW);
    screen.fillTriangle(400, 15, 408, 19, 408, 11, TFT_BLACK);

    screen.fillCircle(430, 15, 8, TFT_YELLOW);
    screen.fillTriangle(430, 15, 438, 19, 438, 11, TFT_BLACK);

    screen.fillCircle(460, 15, 8, TFT_YELLOW);
    screen.fillTriangle(460, 15, 468, 19, 468, 11, TFT_BLACK);
}

void Game::Loop() {
    // Things that happen every loop goes here.
}

void Game::Start() {
    screen.DrawMap(map, map_color);
    pacman.Move(100, 200);
    pacman.Draw(screen);

    enemy.is_pacman = false;

    enemy.Move(150, 200);
    enemy.Draw(screen);
}
