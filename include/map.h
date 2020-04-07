#pragma once

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

#include "misc.h"

class Junction {
 public:
  uint16_t x;
  uint16_t y;

  Junction **adjacent;
  uint8_t id;  // Used for hashing default set to 255 to mark invalid ID

  Junction(uint16_t x, uint16_t y);

  Junction *next(Orientation d);

  Orientation link(Junction *j);
};

class Map {
 private:
  Junction **nodes;
  uint8_t n = 0;
  Map();
  Map(Junction **nodes, uint8_t n);

 public:
  ~Map();

  Point getXY(Junction *startingNode);

  void Generate();

  Junction *GetStart();

  uint8_t GetNodeCount();

  friend class MapBuilder;
};

struct MapBuilder {
  Map *map = new Map();

  uint8_t currentJunction = 0;

  MapBuilder &SetNumJunctions(uint8_t junctions) {
    map->n = junctions;
    return *this;
  }

  MapBuilder &SetJunctionLocations(uint16_t x, uint16_t y) {
    map->nodes[currentJunction] = new Junction(x, y);
    map->nodes[currentJunction]->id = currentJunction;
    currentJunction++;
    return *this;
  }

  MapBuilder &LinkJunctions(uint8_t junctionID1, uint8_t junctionID2) {
    map->nodes[junctionID1]->link(map->nodes[junctionID2]);
    return *this;
  }

  MapBuilder &TestGen() {
    this->SetNumJunctions(9)
        .SetJunctionLocations(20, 120)
        .SetJunctionLocations(50, 120)
        .SetJunctionLocations(50, 300)
        .SetJunctionLocations(50, 60)
        .SetJunctionLocations(120, 120)
        .SetJunctionLocations(120, 300)
        .SetJunctionLocations(120, 60)
        .SetJunctionLocations(240, 60)
        .SetJunctionLocations(240, 300)
        .LinkJunctions(1, 2)
        .LinkJunctions(2, 3)
        .LinkJunctions(2, 4)
        .LinkJunctions(2, 5)
        .LinkJunctions(5, 6)
        .LinkJunctions(5, 7)
        .LinkJunctions(6, 3)
        .LinkJunctions(6, 9)
        .LinkJunctions(7, 4)
        .LinkJunctions(7, 8)
        .LinkJunctions(8, 9);

    return *this;
  }

  Map *Build() { return map; }
};