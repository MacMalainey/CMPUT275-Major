#pragma once

#include <stdint.h>
#include <MCUFRIEND_kbv.h>

struct Screen : public MCUFRIEND_kbv {
    static const uint16_t DISPLAY_WIDTH = 480;
    static const uint16_t DISPLAY_HEIGHT = 320;

//    bool layer0[DISPLAY_WIDTH/2][(DISPLAY_HEIGHT - 30)/2]; // Map
//    uint8_t layer1[DISPLAY_WIDTH][DISPLAY_HEIGHT - 30]; // Pellets
//    uint8_t layer2[DISPLAY_WIDTH][DISPLAY_HEIGHT - 30]; // Entities


    void DrawMap();
    void DrawPellets();

    Screen();
};