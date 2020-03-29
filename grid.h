#pragma once

#include "map.h"
#include "entities.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

uint8_t DIVISIONS = 10;

uint16_t cellSizeX = DISPLAY_WIDTH / DIVISIONS;
uint16_t cellSizeY = DISPLAY_HEIGHT / DIVISIONS;

class Cell {
 private:
  Drawable Ghosts;
  Pellet Pellets;
};

class Grid {
  friend class Cell;

 private:
  //Cell Grid;

  Point getGridPos(uint16_t x, uint16_t y);
  
 public:
  Grid();

  void addGhost(Drawable ghost);
  void removeGhost(Drawable ghost);
  void ghostMoved(Drawable ghost);

  void addPellet(Pellet pellet);
  void removePellet(Pellet pellet);

  void update();
};
