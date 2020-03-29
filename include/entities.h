#pragma once

#include <MCUFRIEND_kbv.h>

#include <stdint.h>

class Drawable {
public:
    Drawable(uint16_t x = 0, uint16_t y = 0) :
    start_x(x),
    start_y(y), offset_x(
    0), offset_y(0) {

    }

    void Move(uint16_t new_x, uint16_t new_y) {
        offset_x += new_x;
        offset_y += new_y;
    }

    void Reset() {
        offset_x = 0;
        offset_y = 0;
    }

    virtual void Draw(Screen &) = 0;

    uint16_t start_x;
    uint16_t start_y;

    int16_t offset_x;
    int16_t offset_y;

    uint16_t color;
};

struct PlayerCharacter : public Drawable {

    void Draw(Screen &screen) final {
        Serial.print("Drawing character at: ");
        Serial.print(start_x + offset_x);
        if (is_pacman) {
            screen.fillCircle(start_x + offset_x, start_y + offset_y, 4, TFT_YELLOW);
            screen.fillTriangle(start_x + offset_x, start_y + offset_y, start_x + offset_x + 4, start_y + offset_y + 2,
                                start_x + offset_x + 4, start_y + offset_y - 2, TFT_BLACK);
        } else{
            screen.fillCircle(start_x + offset_x, start_y + offset_y, 7, TFT_RED);
        }

    }

    bool is_pacman = true;
};