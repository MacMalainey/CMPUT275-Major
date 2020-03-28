#pragma once

#include <MCUFRIEND_kbv.h>

enum Orientation { NORTH = 0, SOUTH, EAST, WEST, N_ORIENT };

class Junction {
  // We could create getters instead of using a friend class
  // But this is easier
  friend class Map;

 private:
  uint16_t x;
  uint16_t y;

  Junction** adjacent;
  uint8_t id;  // Used for hashing default set to 255 to mark invalid ID

 public:
  Junction(uint16_t x, uint16_t y);

  Junction* next(Orientation d);
  Orientation link(Junction* j);
};

class Map {
 private:
  Junction** nodes;
  uint8_t n = 0;

 public:
  Map(Junction** nodes, uint8_t n);
  ~Map();

  void draw(MCUFRIEND_kbv& canvas, uint16_t color);
};

Map* buildDemoMap();
