#pragma once

#include "screen.h"
#include "map.h"
#include "assets.h"
#include "entities.h"

class Game {

public:
    Game();
    ~Game();
    void Start();
    void Loop();
    bool is_running = true;

private:

    int current_lives = 3;
    uint16_t score = 0;

    void updateScore();
    void drawLives();
    void decrementLives();

    Map* map;
    uint16_t map_color;
    Screen screen;

    PlayerCharacter pacman;
    PlayerCharacter enemy;

};