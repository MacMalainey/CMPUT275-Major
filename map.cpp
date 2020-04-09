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

  // Serial.print(x);
  // Serial.print(" ,");
  // Serial.print(y);

  // Serial.print("  ");

  // Serial.print(j->adjacent[op]->x);
  // Serial.print(", ");
  // Serial.println(j->adjacent[op]->y);
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
}
void MapBuilder::SetJunctionLocations(uint8_t id, uint16_t x, uint16_t y) {
  auto newJunction = new Junction(x, y);
  newJunction->id = id;
  copy_arr[id - 1] = newJunction;
  Serial.print("Added new junction at ");
  Serial.print(copy_arr[id - 1]->x);
  Serial.print(" ");
  Serial.println(copy_arr[id - 1]->y);
  Serial.println(copy_arr[id - 1]->id);
}
void MapBuilder::LinkJunctions(uint8_t junctionID1, uint8_t junctionID2) {
  links[linkCount++] = Point{.x = junctionID1, .y = junctionID2};
}
void MapBuilder::TestGen() {
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
Map *MapBuilder::Build() {
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
