#pragma once

#include <MCUFRIEND_kbv.h>
#include <Arduino.h>

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

public:
    Map();

    Map(Junction **nodes, uint8_t n);

    ~Map();

    Point getSpawnXY();

    void Generate();

    Junction *GetStart();

    uint8_t GetNodeCount();
};
