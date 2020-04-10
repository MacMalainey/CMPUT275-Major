/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include "include/map.h"

Map::Map(Junction **nodes, uint8_t n) {
  this->n = n;
  this->nodes = nodes;
}

Junction *Map::GetStart() { return nodes[0]; }

uint8_t Map::GetNodeCount() { return n; }

Junction::Junction(uint16_t x, uint16_t y) {
  this->x = x;
  this->y = y;

  adjacent = new Junction *[N_ORIENT];

  for (uint8_t i = 0; i < N_ORIENT; i++) {
    adjacent[i] = nullptr;
  }
}

Junction *Junction::next(Orientation d) { return adjacent[d]; }

Orientation Junction::link(Junction *j) {
  int16_t dx = j->x - x;
  int16_t dy = j->y - y;

  auto d = (Orientation)0;
  auto op = (Orientation)0;

  if ((dx != 0 && dy != 0) || (dx == 0 && dy == 0)) {
    // Either a diagonal addition
    // Or is at the same position, both invalid
    return N_ORIENT;
  } else if (dx > 0) {
    d = EAST;
    op = WEST;
  } else if (dx < 0) {
    d = WEST;
    op = EAST;
  } else if (dy < 0) {
    d = NORTH;
    op = SOUTH;
  } else if (dy > 0) {
    d = SOUTH;
    op = NORTH;
  }

  // If this is linked with another junction we need to sever the link
  if (adjacent[d] != nullptr) {
    adjacent[d]->adjacent[op] = nullptr;
  }

  adjacent[d] = j;
  j->adjacent[op] = this;

  return d;
}

Map::~Map() { delete[] nodes; }

Point Map::getXY(Junction *node) {
  Point startingPoint = {node->x, node->y};
  return startingPoint;
}

void MapBuilder::SetJunctionCount(uint8_t count) {
  junctionCount = count;
  copy_arr = new Junction *[count];
  links = new Point[count * 4];
  // TODO: handle potential memory leak
}
void MapBuilder::SetJunctionLocations(uint8_t id, uint16_t x, uint16_t y) {
  auto newJunction = new Junction(x, y);
  newJunction->id = id;
  copy_arr[id] = newJunction;
}
void MapBuilder::LinkJunctions(uint8_t junctionID1, uint8_t junctionID2) {
  links[linkCount++] = Point{.x = junctionID1, .y = junctionID2};
}
void MapBuilder::TestGen() {
  SetJunctionCount(9);
  LinkJunctions(0, 1);
  LinkJunctions(1, 2);
  LinkJunctions(1, 3);
  LinkJunctions(1, 4);
  LinkJunctions(4, 5);
  LinkJunctions(4, 6);
  LinkJunctions(5, 2);
  LinkJunctions(5, 8);
  LinkJunctions(6, 3);
  LinkJunctions(6, 7);
  LinkJunctions(7, 8);
  SetJunctionLocations(0, 20, 120);
  SetJunctionLocations(1, 50, 120);
  SetJunctionLocations(2, 50, 300);
  SetJunctionLocations(3, 50, 60);
  SetJunctionLocations(4, 120, 120);
  SetJunctionLocations(5, 120, 300);
  SetJunctionLocations(6, 120, 60);
  SetJunctionLocations(7, 240, 60);
  SetJunctionLocations(8, 240, 300);
}

Map *MapBuilder::Build() {
  for (uint16_t i = 0; i < linkCount; i++) {
    auto currentLink = links[i];
    copy_arr[currentLink.x]->link(copy_arr[currentLink.y]);
  }
  return new Map(copy_arr, junctionCount);
}
