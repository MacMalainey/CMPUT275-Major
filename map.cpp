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
  Serial.print("Added new junction at ");
  Serial.print(copy_arr[id]->x);
  Serial.print(" ");
  Serial.println(copy_arr[id]->y);
  Serial.println(copy_arr[id]->id);
}
void MapBuilder::LinkJunctions(uint8_t junctionID1, uint8_t junctionID2) {
  links[linkCount++] = Point{.x = junctionID1, .y = junctionID2};
}
void MapBuilder::TestGen() {
  // SetJunctionCount(9);
  // LinkJunctions(0, 1);
  // LinkJunctions(1, 2);
  // LinkJunctions(1, 3);
  // LinkJunctions(1, 4);
  // LinkJunctions(4, 5);
  // LinkJunctions(4, 6);
  // LinkJunctions(5, 2);
  // LinkJunctions(5, 8);
  // LinkJunctions(6, 3);
  // LinkJunctions(6, 7);
  // LinkJunctions(7, 8);
  // SetJunctionLocations(0, 20, 120);
  // SetJunctionLocations(1, 50, 120);
  // SetJunctionLocations(2, 50, 300);
  // SetJunctionLocations(3, 50, 60);
  // SetJunctionLocations(4, 120, 120);
  // SetJunctionLocations(5, 120, 300);
  // SetJunctionLocations(6, 120, 60);
  // SetJunctionLocations(7, 240, 60);
  // SetJunctionLocations(8, 240, 300);

int16_t center_x = 240;

SetJunctionCount(70);

SetJunctionLocations(0, center_x - 180 , 60);
SetJunctionLocations(1, center_x + 180 , 60);
SetJunctionLocations(2, center_x - 90 , 60);
SetJunctionLocations(3, center_x + 90 , 60);
SetJunctionLocations(4, center_x - 10 , 60);
SetJunctionLocations(5, center_x + 10 , 60);

SetJunctionLocations(6, center_x - 180 , 90);
SetJunctionLocations(7, center_x + 180 , 90);
SetJunctionLocations(8, center_x - 120 , 90);
SetJunctionLocations(9, center_x + 120 , 90);
SetJunctionLocations(10, center_x - 90 , 90);
SetJunctionLocations(11, center_x + 90 , 90);
SetJunctionLocations(12, center_x - 10 , 90);
SetJunctionLocations(13, center_x + 10 , 90);

SetJunctionLocations(14, center_x - 120 , 120);
SetJunctionLocations(15, center_x + 120 , 120);
SetJunctionLocations(16, center_x - 80 , 120);
SetJunctionLocations(17, center_x + 80 , 120);
SetJunctionLocations(18, center_x - 30 , 120);
SetJunctionLocations(19, center_x + 30 , 120);
SetJunctionLocations(20, center_x - 10 , 120);
SetJunctionLocations(21, center_x + 10 , 120);

SetJunctionLocations(22, center_x - 180 , 160);
SetJunctionLocations(23, center_x + 180 , 160);
SetJunctionLocations(24, center_x - 120 , 160);
SetJunctionLocations(25, center_x + 120 , 160);
SetJunctionLocations(26, center_x - 30 , 160);
SetJunctionLocations(27, center_x + 30 , 160);
SetJunctionLocations(28, center_x - 10 , 160);
SetJunctionLocations(29, center_x + 10 , 160);

SetJunctionLocations(30, center_x, 160);
SetJunctionLocations(31, center_x, 170);

SetJunctionLocations(32, center_x - 80 , 170);
SetJunctionLocations(33, center_x + 80 , 170);
SetJunctionLocations(34, center_x - 30 , 170);
SetJunctionLocations(35, center_x + 30 , 170);
SetJunctionLocations(36, center_x - 10 , 170);
SetJunctionLocations(37, center_x + 10 , 170);
SetJunctionLocations(38, center_x - 30 , 180);
SetJunctionLocations(39, center_x + 30 , 180);

SetJunctionLocations(40, center_x - 120 , 200);
SetJunctionLocations(41, center_x + 120 , 200);
SetJunctionLocations(42, center_x - 80 , 200);
SetJunctionLocations(43, center_x + 80 , 200);
SetJunctionLocations(44, center_x - 30 , 200);
SetJunctionLocations(45, center_x + 30 , 200);
SetJunctionLocations(46, center_x - 10 , 200);
SetJunctionLocations(47, center_x + 10 , 200);

SetJunctionLocations(48, center_x - 180 , 230);
SetJunctionLocations(49, center_x + 180 , 230);
SetJunctionLocations(50, center_x - 120 , 230);
SetJunctionLocations(51, center_x + 120 , 230);
SetJunctionLocations(52, center_x - 60 , 230);
SetJunctionLocations(53, center_x + 60 , 230);
SetJunctionLocations(54, center_x - 10 , 230);
SetJunctionLocations(55, center_x + 10 , 230);

SetJunctionLocations(56, center_x - 120 , 260);
SetJunctionLocations(57, center_x + 120 , 260);
SetJunctionLocations(58, center_x - 90 , 260);
SetJunctionLocations(59, center_x + 90 , 260);
SetJunctionLocations(60, center_x - 60 , 260);
SetJunctionLocations(61, center_x + 60 , 260);
SetJunctionLocations(62, center_x - 10 , 260);
SetJunctionLocations(63, center_x + 10 , 260);

SetJunctionLocations(64, center_x - 180 , 290);
SetJunctionLocations(65, center_x + 180 , 290);
SetJunctionLocations(66, center_x - 90 , 290);
SetJunctionLocations(67, center_x + 90 , 290);
SetJunctionLocations(68, center_x - 10 , 290);
SetJunctionLocations(69, center_x + 10 , 290);


LinkJunctions(0, 2);
LinkJunctions(0, 6);
LinkJunctions(2, 10);
LinkJunctions(2, 4);
LinkJunctions(4, 5);
LinkJunctions(4, 12);
LinkJunctions(10, 12);
LinkJunctions(10, 8);
LinkJunctions(6, 8);
LinkJunctions(14, 8);
LinkJunctions(6, 22);
LinkJunctions(22, 24);
LinkJunctions(24, 14);
LinkJunctions(14, 16);
LinkJunctions(32, 16);
LinkJunctions(18, 16);
LinkJunctions(18, 20);
LinkJunctions(20, 12);
LinkJunctions(18, 26);
LinkJunctions(26, 34);
LinkJunctions(26, 28);
LinkJunctions(28, 30);
LinkJunctions(28, 36);
LinkJunctions(31, 30);
LinkJunctions(29, 30);
LinkJunctions(29, 37);
LinkJunctions(31, 37);
LinkJunctions(31, 36);
LinkJunctions(32, 34);
LinkJunctions(38, 34);
LinkJunctions(38, 39);
LinkJunctions(38, 44);
LinkJunctions(46, 44);
LinkJunctions(42, 44);
LinkJunctions(32, 42);
LinkJunctions(40, 42);
LinkJunctions(24, 40);
LinkJunctions(50, 40);
LinkJunctions(50, 48);
LinkJunctions(22, 48);
LinkJunctions(50, 52);
LinkJunctions(52, 54);
LinkJunctions(54, 46);
LinkJunctions(54, 55);
LinkJunctions(56, 50);
LinkJunctions(56, 58);
LinkJunctions(58, 60);
LinkJunctions(60, 52);
LinkJunctions(60, 62);
LinkJunctions(58, 66);
LinkJunctions(66, 64);
LinkJunctions(66, 68);
LinkJunctions(64, 48);
LinkJunctions(68, 62);
LinkJunctions(5, 13);
LinkJunctions(5, 3);
LinkJunctions(3, 11);
LinkJunctions(3, 1);
LinkJunctions(1, 7);
LinkJunctions(7, 23);
LinkJunctions(7, 9);
LinkJunctions(9, 11);
LinkJunctions(9, 15);
LinkJunctions(11, 13);
LinkJunctions(13, 21);
LinkJunctions(21, 19);
LinkJunctions(19, 27);
LinkJunctions(19, 17);
LinkJunctions(17, 15);
LinkJunctions(17, 33);
LinkJunctions(33, 35);
LinkJunctions(35, 39);
LinkJunctions(35, 27);
LinkJunctions(27, 29);
LinkJunctions(39, 45);
LinkJunctions(45, 47);
LinkJunctions(45, 43);
LinkJunctions(47, 55);
LinkJunctions(55, 53);
LinkJunctions(53, 51);
LinkJunctions(53, 61);
LinkJunctions(51, 41);
LinkJunctions(51, 49);
LinkJunctions(49, 23);
LinkJunctions(23, 25);
LinkJunctions(25, 15);
LinkJunctions(25, 41);
LinkJunctions(41, 43);
LinkJunctions(43, 33);
LinkJunctions(49, 65);
LinkJunctions(65, 67);
LinkJunctions(67, 59);
LinkJunctions(59, 57);
LinkJunctions(57, 51);
LinkJunctions(59, 61);
LinkJunctions(61, 63);
LinkJunctions(63, 69);
LinkJunctions(69, 68);
LinkJunctions(69, 67);
}

Map *MapBuilder::Build() {
  for (uint16_t i = 0; i < linkCount; i++) {
    auto currentLink = links[i];
    copy_arr[currentLink.x]->link(copy_arr[currentLink.y]);

    auto currentJunction = copy_arr[currentLink.x];

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
