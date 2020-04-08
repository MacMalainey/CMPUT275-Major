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

  void Generate();

  Junction *GetStart();

  uint8_t GetNodeCount();
};

struct MapBuilder {
  Junction **copy_arr;
  Point *links;

  uint8_t junctionCount = 0;
  uint16_t linkCount = 0;

  void SetJunctionCount(uint8_t count) {
    copy_arr = new Junction *[count];
    links = new Point[count * 4];
  }

  void SetJunctionLocations(uint16_t x, uint16_t y) {
    auto newJunction = new Junction(x, y);
    newJunction->id = junctionCount;
    copy_arr[junctionCount] = newJunction;
    Serial.print("Added new junction at ");
    Serial.print(copy_arr[junctionCount]->x);
    Serial.print(" ");
    Serial.println(copy_arr[junctionCount]->y);
    Serial.println(copy_arr[junctionCount]->id);

    junctionCount++;
  }

  void LinkJunctions(uint8_t junctionID1, uint8_t junctionID2) {
    links[linkCount++] = Point{.x = junctionID1, .y = junctionID2};
  }

  void TestGen() {
    SetJunctionCount(9);
    SetJunctionLocations(20, 120);
    SetJunctionLocations(50, 120);
    SetJunctionLocations(50, 300);
    SetJunctionLocations(50, 60);
    SetJunctionLocations(120, 120);
    SetJunctionLocations(120, 300);
    SetJunctionLocations(120, 60);
    SetJunctionLocations(240, 60);
    SetJunctionLocations(240, 300);
    LinkJunctions(1u, 2u);
    LinkJunctions(2u, 3u);
    LinkJunctions(2u, 4u);
    LinkJunctions(2u, 5u);
    LinkJunctions(5u, 6u);
    LinkJunctions(5u, 7u);
    LinkJunctions(6u, 3u);
    LinkJunctions(6u, 9u);
    LinkJunctions(7u, 4u);
    LinkJunctions(7u, 8u);
    LinkJunctions(8u, 9u);
  }

  Map *Build() {
    for (auto i = 0; i < linkCount; i++) {
      auto currentLink = links[i];
      copy_arr[currentLink.x]->link(copy_arr[currentLink.y]);
      Serial.print("Linking: ");
      Serial.print(currentLink.x);
      Serial.print(" To ");
      Serial.println(currentLink.y);
    }

    return new Map(copy_arr, junctionCount);
  }
};