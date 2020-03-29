#pragma once

#include <stdint.h>
#include <MCUFRIEND_kbv.h>

#include "map.h"
#include "misc.h"

struct Screen : public MCUFRIEND_kbv {
    static const uint16_t DISPLAY_WIDTH = 480;
    static const uint16_t DISPLAY_HEIGHT = 320;

    Screen();

    void DrawMap(Map &map, uint16_t color)
};