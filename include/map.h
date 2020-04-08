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
    junctionCount = count;
    copy_arr = new Junction *[count];
    links = new Point[count * 4];
  }

  void SetJunctionLocations(uint8_t id, uint16_t x, uint16_t y) {
    auto newJunction = new Junction(x, y);
    newJunction->id = id;
    copy_arr[id - 1] = newJunction;
    Serial.print("Added new junction at ");
    Serial.print(copy_arr[id - 1]->x);
    Serial.print(" ");
    Serial.println(copy_arr[id - 1]->y);
    Serial.println(copy_arr[id - 1]->id);
  }

  void LinkJunctions(uint8_t junctionID1, uint8_t junctionID2) {
    links[linkCount++] = Point{.x = junctionID1, .y = junctionID2};
  }

  void TestGen() {
    SetJunctionCount(9);
    LinkJunctions(1, 2);
    LinkJunctions(2, 3);
    LinkJunctions(2, 4);
    LinkJunctions(2, 5);
    LinkJunctions(5, 6);
    LinkJunctions(5, 7);
    LinkJunctions(6, 3);
    LinkJunctions(6, 9);
    LinkJunctions(7, 4);
    LinkJunctions(7, 8);
    LinkJunctions(8, 9);
    SetJunctionLocations(1, 20, 120);
    SetJunctionLocations(2, 50, 120);
    SetJunctionLocations(3, 50, 300);
    SetJunctionLocations(4, 50, 60);
    SetJunctionLocations(5, 120, 120);
    SetJunctionLocations(6, 120, 300);
    SetJunctionLocations(7, 120, 60);
    SetJunctionLocations(8, 240, 60);
    SetJunctionLocations(9, 240, 300);
  }

  Map *Build() {
    for (uint16_t i = 0; i < linkCount; i++) {
      auto currentLink = links[i];
      copy_arr[currentLink.x - 1]->link(copy_arr[currentLink.y - 1]);

      auto currentJunction = copy_arr[currentLink.x - 1];

      for (uint8_t i = 0; i < N_ORIENT; i++) {
        if (currentJunction->adjacent[i] != nullptr) {
          Serial.print("Linked: ");
          Serial.print(currentJunction->id);
          Serial.print(" To ");
          Serial.println(currentJunction->adjacent[i]->id);
        }
      }
    }
    return new Map(copy_arr, junctionCount);
  }
};