/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

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
  Junction **nodes = nullptr;
  uint8_t n = 0;

 public:
  Map(Junction **nodes, uint8_t n);
  ~Map();

  Point getXY(Junction *startingNode);

  Junction *GetStart();

  uint8_t GetNodeCount();
};

struct MapBuilder {
  Junction **copy_arr;
  Point *links;

  uint8_t junctionCount = 0;
  uint16_t linkCount = 0;

  void SetJunctionCount(uint8_t count);

  void SetJunctionLocations(uint8_t id, uint16_t x, uint16_t y);

  void LinkJunctions(uint8_t junctionID1, uint8_t junctionID2);

  void TestGen();

  Map *Build();
};