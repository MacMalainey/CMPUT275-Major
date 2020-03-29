#pragma once

#include <MCUFRIEND_kbv.h>

#include <stdint.h>

#include "screen.h"

class Drawable
{
public:
    Drawable(uint16_t start_x = 0, uint16_t start_y = 0) : x(start_x), y(start_y)
    {
    }

    void Move(uint16_t new_x, uint16_t new_y)
    {
        x += new_x;
        y += new_y;
    }

    virtual void Draw(Screen &) = 0;

    Orientation queuedMovement;
    int immediateMovement[4];

    uint16_t x;
    uint16_t y;

    uint16_t color;

    bool isVisible = true;
};

struct Pellet : public Drawable
{

    void Draw(Screen &screen) final
    {
        if(isVisible) {
            screen.fillCircle(x, y, 1, TFT_WHITE);
        }
    }

};

struct PlayerCharacter : public Drawable
{

    void Draw(Screen &screen) final
    {
        if (is_pacman)
        {
            screen.fillCircle(x, y, 4, TFT_YELLOW);
            screen.fillTriangle(x, y, x + 4, y + 2,
                                x + 4, y - 2, TFT_BLACK);
        }
        else
        {
            screen.fillCircle(x, y, 7, TFT_RED);
        }
    }

    bool is_pacman = true;
};