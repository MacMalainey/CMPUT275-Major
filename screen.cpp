#include "include/screen.h"

Screen::Screen() {
  uint16_t ID = this->readID();
  this->begin(ID);
  this->fillScreen(TFT_BLACK);
  this->setRotation(1);
  this->setTextSize(2);
}

void Screen::DrawMap(Map &map, uint16_t color) {
  uint16_t c_width = 10;
  uint16_t c_width_half = c_width / 2;

  bool touched[map.GetNodeCount()] = {false};

  Queue<Junction *> events;
  events.push(map.GetStart());
  touched[0] = true;

  // canvas.setTextColor(color);
  // canvas.setTextSize(3);

  while (events.size() > 0) {
    Junction *j = events.pop();

    Serial.print("Drawing: ");
    Serial.println(j->id);

    fillCircle(j->x, j->y, 8, color);
    for (uint8_t i = 0; i < N_ORIENT; i++) {
      if (j->next((Orientation)i) != nullptr) {
        Junction *a = j->adjacent[i];
        // Ensures this is only added to the queue once
        if (!touched[a->id]) {
          touched[a->id] = true;
          events.push(a);
          Serial.print("PUSHED: ");
          Serial.println(a->id);
        }

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

          drawLine(x_j0, y_j, x_a0, y_a, color);
          drawLine(x_j1, y_j, x_a1, y_a, color);
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

          drawLine(x_j, y_j0, x_a, y_a0, color);
          drawLine(x_j, y_j1, x_a, y_a1, color);
        }

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

        drawLine(x0, y0, x1, y1, color);
      }
    }
  }
}