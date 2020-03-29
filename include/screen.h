#pragma once

#include <stdint.h>
#include <MCUFRIEND_kbv.h>

struct Screen : public MCUFRIEND_kbv {
    static const uint16_t DISPLAY_WIDTH = 480;
    static const uint16_t DISPLAY_HEIGHT = 320;

    Screen();
};