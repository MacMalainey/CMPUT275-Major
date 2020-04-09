/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#pragma once

#include <MCUFRIEND_kbv.h>
#include <stdint.h>

#include "map.h"

class Map;
struct Screen : public MCUFRIEND_kbv {
  static const uint16_t DISPLAY_WIDTH = 480;
  static const uint16_t DISPLAY_HEIGHT = 320;

  Screen();

  void DrawMap(Map *map, uint16_t color);
};