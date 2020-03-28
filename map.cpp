#include "map.h"

#include <Arduino.h>

#include "assets.h"
#include "misc.h"

#define DEBUG_DRAW true

uint16_t c_width = 20;
uint16_t c_width_half = c_width / 2;

Map::Map(Junction** nodes, uint8_t n) {
  this->n = n;
  this->nodes = new Junction*[n];
  for (uint8_t i = 0; i < n; i++) {
    this->nodes[i] = nodes[i];
    this->nodes[i]->id =
        i;  // Set id to make certain all Junctions have unique values
  }
}

void Map::draw(MCUFRIEND_kbv& canvas, uint16_t color) {
  // Easily can draw using a BFS
  bool touched[n] = {false};

  Queue<Junction*> events;
  events.push(nodes[0]);
  touched[0] = true;

  // canvas.setTextColor(color);
  // canvas.setTextSize(3);

  while (events.size() > 0) {
    Junction* j = events.pop();
    Serial.print("POPPED: ");
    Serial.println(j->id);
#if (DEBUG_DRAW)
    canvas.fillCircle(j->x, j->y, 8, color);
// canvas.setCursor(j->x, j->y);
// canvas.print(j->id);
#endif
    for (uint8_t i = 0; i < N_ORIENT; i++) {
      if (j->next((Orientation)i) != NULL) {
        Junction* a = j->adjacent[i];
        // Ensures this is only added to the queue once
        if (!touched[a->id]) {
          touched[a->id] = true;
          events.push(a);
          Serial.print("PUSHED: ");
          Serial.println(a->id);
        }

#if (DEBUG_DRAW)
        // Serial.print(j->id);
        // Serial.print(", ");
        // Serial.print(a->id);
        // Serial.print(", ");
        // Serial.println(i);

        if (j->x == a->x) {  // vertical corridor
          uint32_t x_j0 = j->x - c_width_half;
          uint32_t x_a0 = a->x - c_width_half;
          uint32_t x_j1 = j->x + c_width_half;
          uint32_t x_a1 = a->x + c_width_half;

          uint32_t jy = j->y;
          uint32_t ay = a->y;

          // make sure we're drawing in the right direction
          if (jy > ay) {
            uint32_t temp = jy;
            jy = ay;
            ay = temp;
          }

          uint32_t y_j = jy + c_width_half;
          uint32_t y_a = ay - c_width_half;

          canvas.drawLine(x_j0, y_j, x_a0, y_a, color);
          canvas.drawLine(x_j1, y_j, x_a1, y_a, color);
        } else {  // horizontal corridor
          uint32_t y_j0 = j->y - c_width_half;
          uint32_t y_a0 = a->y - c_width_half;
          uint32_t y_j1 = j->y + c_width_half;
          uint32_t y_a1 = a->y + c_width_half;

          uint32_t jx = j->x;
          uint32_t ax = a->x;

          // make sure we're drawing in the right direction
          if (jx > ax) {
            uint32_t temp = jx;
            jx = ax;
            ax = temp;
          }

          uint32_t x_j = jx + c_width_half;
          uint32_t x_a = ax - c_width_half;

          canvas.drawLine(x_j, y_j0, x_a, y_a0, color);
          canvas.drawLine(x_j, y_j1, x_a, y_a1, color);
        }
// canvas.drawLine(j->x, j->y, a->x, a->y, color);
#endif
      } else {
        // The direction leads no where, so we need to draw a line.
        uint32_t x0, x1, y0, y1;

        if (i == NORTH) {
          x0 = j->x - c_width_half;
          x1 = j->x + c_width_half;

          y0 = j->y - c_width_half;
          y1 = y0;
        } else if (i == SOUTH) {
          x0 = j->x - c_width_half;
          x1 = j->x + c_width_half;

          y0 = j->y + c_width_half;
          y1 = y0;
        } else if (i == EAST) {
          x0 = j->x + c_width_half;
          x1 = x0;

          y0 = j->y - c_width_half;
          y1 = j->y + c_width_half;
        } else if (i == WEST) {
          x0 = j->x - c_width_half;
          x1 = x0;

          y0 = j->y - c_width_half;
          y1 = j->y + c_width_half;
        }

        canvas.drawLine(x0, y0, x1, y1, color);
      }
    }
  }
}

Junction::Junction(uint16_t x, uint16_t y) {
  this->x = x;
  this->y = y;

  adjacent = new Junction*[N_ORIENT];

  for (uint8_t i = 0; i < N_ORIENT; i++) {
    adjacent[i] = NULL;
  }
}

Junction* Junction::next(Orientation d) { return adjacent[d]; }

Orientation Junction::link(Junction* j) {
  int16_t dx = j->x - x;
  int16_t dy = j->y - y;

  Orientation d;
  Orientation op;

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
  if (adjacent[d] != NULL) {
    adjacent[d]->adjacent[op] = NULL;
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

Map* buildDemoMap() {
  // Most left is 10, top of screen is 40
  // Most right is 469, bottom of screen is 309

  Junction* j1 = new Junction(10, 40);
  Junction* j2 = new Junction(70, 40);
  Junction* j3 = new Junction(150, 40);

  Junction* j4 = new Junction(10, 80);
  Junction* j6 = new Junction(70, 80);
  Junction* j8 = new Junction(150, 80);

  Junction* j5 = new Junction(10, 120);
  Junction* j7 = new Junction(70, 120);
  Junction* j9 = new Junction(100, 120);

  // Junction* j2 = new Junction(50, 120);
  // Junction* j3 = new Junction(50, 300);
  // Junction* j4 = new Junction(50, 40);
  // Junction* j5 = new Junction(120, 120);
  // Junction* j6 = new Junction(120, 300);
  // Junction* j7 = new Junction(120, 40);
  // Junction* j8 = new Junction(240, 40);
  // Junction* j9 = new Junction(240, 300);

  j1->link(j2);
  j1->link(j4);

  j2->link(j3);
  j2->link(j6);

  j3->link(j8);

  j4->link(j5);
  j4->link(j6);

  j5->link(j7);

  j6->link(j7);
  j6->link(j8);
  // j2->link(j3);
  // j2->link(j4);
  // j2->link(j5);

  // j5->link(j6);
  // j5->link(j7);

  // j6->link(j3);
  // j6->link(j9);

  // j7->link(j4);
  // j7->link(j8);

  // j8->link(j9);

  Junction* copy_arr[] = {j1, j2, j3, j4, j5, j6, j7, j8, j9};

  return new Map(copy_arr, 9);
}
