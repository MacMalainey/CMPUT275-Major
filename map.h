#pragma once

#include <MCUFRIEND_kbv.h>

enum Orientation { NORTH = 0, SOUTH, EAST, WEST, N_ORIENT };

struct Point {
  uint16_t x;
  uint16_t y;
};

class Junction {
  // We could create getters instead of using a friend class
  // But this is easier
  friend class Map;

 private:
  uint16_t x;
  uint16_t y;

  Junction** adjacent;
  uint8_t id;  // Used for hashing default set to 255 to mark invalid ID

<<<<<<< HEAD
    public:

    Junction(uint16_t x, uint16_t y);
=======
 public:
  Junction(uint16_t x, uint16_t y);
>>>>>>> 9b5a7dbb887b25c862b6f43a7149ecc6aaefc161

  Junction* next(Orientation d);
  Orientation link(Junction* j);
};

class Map {
 private:
  Junction** nodes;
  uint8_t n;

 public:
  Map(Junction** nodes, uint8_t n);
  ~Map();

<<<<<<< HEAD
    Point getSpawnXY();

    void draw(MCUFRIEND_kbv canvas, uint16_t color);
=======
  void draw(MCUFRIEND_kbv& canvas, uint16_t color);
>>>>>>> 9b5a7dbb887b25c862b6f43a7149ecc6aaefc161
};

Map* buildDemoMap();
