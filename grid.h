#pragma once

#include "map.h"
#include "entities.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

///////////////////////////////////////////////////////////////////////////////
// Cell class
///////////////////////////////////////////////////////////////////////////////

class Cell {
  friend class Row;
  friend class Grid;

 private:
  Drawable ghosts;
  Pellet pellets;

  uint16_t id;

  Cell(uint16_t id);

 public:
  uint16_t getID();
};

///////////////////////////////////////////////////////////////////////////////
// Row class
///////////////////////////////////////////////////////////////////////////////

class Row {
  friend class Cell;
  friend class Grid;

 private:
  Cell** cells;
  uint8_t divisions;

  Row(uint8_t divisions);
  ~Row();

 public:
  Cell* getCell(uint8_t index);
};

///////////////////////////////////////////////////////////////////////////////
// Grid class
///////////////////////////////////////////////////////////////////////////////

class Grid {
  friend class Row;
  friend class Cell;

 private:
  Row** rows;
  uint8_t divisions;

  Point getGridPos(uint16_t x, uint16_t y);
  
 public:
  Grid(uint8_t divisions);
  ~Grid();

  Row* getRow(uint8_t index);

  void addGhost(Drawable ghost);
  void removeGhost(Drawable ghost);
  void ghostMoved(Drawable ghost);

  void addPellet(Pellet pellet);
  void removePellet(Pellet pellet);

  void update();
};

Grid* testGrid(uint8_t divisions);
