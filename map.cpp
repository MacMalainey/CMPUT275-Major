#include "include/map.h"

Map::Map() {
    n = 0;
    nodes = nullptr;
}

Junction *Map::GetStart() {
    return nodes[0];
}

uint8_t Map::GetNodeCount() {
    return n;
}

Junction::Junction(uint16_t x, uint16_t y) {
    this->x = x;
    this->y = y;

    adjacent = new Junction *[N_ORIENT];

    for (uint8_t i = 0; i < N_ORIENT; i++) {
        adjacent[i] = nullptr;
    }
}

Junction *Junction::next(Orientation d) {
    return adjacent[d];
}

Orientation Junction::link(Junction *j) {

    int16_t dx = j->x - x;
    int16_t dy = j->y - y;

    auto d = (Orientation) 0;
    auto op = (Orientation) 0;

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

Map::~Map() {
    delete[] nodes;
}

Point Map::getSpawnXY() {
    Junction *startingNode = this->nodes[0];
    Point startingPoint = {startingNode->x, startingNode->y};
    return startingPoint;
}

void Map::Generate() {
    auto j1 = new Junction(20, 120);
    auto j2 = new Junction(50, 120);
    auto j3 = new Junction(50, 300);
    auto j4 = new Junction(50, 60);
    auto j5 = new Junction(120, 120);
    auto j6 = new Junction(120, 300);
    auto j7 = new Junction(120, 60);
    auto j8 = new Junction(240, 60);
    auto j9 = new Junction(240, 300);

    j1->link(j2);

    j2->link(j3);
    j2->link(j4);
    j2->link(j5);

    j5->link(j6);
    j5->link(j7);

    j6->link(j3);
    j6->link(j9);

    j7->link(j4);
    j7->link(j8);

    j8->link(j9);

    Junction *copy_arr[] = {j1, j2, j3, j4, j5, j6, j7, j8, j9};

    this->n = 9;
    this->nodes = new Junction *[n];
    for (uint8_t i = 0; i < n; i++) {
        this->nodes[i] = copy_arr[i];
        this->nodes[i]->id = i; // Set id to make certain all Junctions have unique values
    }
}
