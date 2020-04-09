/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#pragma once

#include "entities.h"
#include "misc.h"

///////////////////////////////////////////////////////////////////////////////
// Cell class
///////////////////////////////////////////////////////////////////////////////

class Cell {
  friend class Row;
  friend class Grid;

 private:
  uint16_t id;

  Cell(uint16_t id);

 public:
  uint16_t getID();

  LinkedList<Pellet> pellets;
};

///////////////////////////////////////////////////////////////////////////////
// Row class
///////////////////////////////////////////////////////////////////////////////

class Row {
  friend class Cell;
  friend class Grid;

 private:
  Cell **cells;
  uint8_t divisions;

  Row(uint8_t divisions, uint16_t num_cells);
  ~Row();

 public:
  Cell *getCell(uint8_t index);
};

///////////////////////////////////////////////////////////////////////////////
// Grid class
///////////////////////////////////////////////////////////////////////////////

class Grid {
  friend class Row;
  friend class Cell;

 private:
  Row **rows;

  Point getGridPos(uint16_t x, uint16_t y);

 public:
  Grid();
  ~Grid();

  uint16_t sizeX;
  uint16_t sizeY;
  uint8_t divisions;
  uint16_t num_cells = 0;

  void Generate(uint8_t divisions);

  Row *getRow(uint8_t index);

  uint8_t getRowIndex(uint16_t x);
  uint8_t getCellIndex(uint16_t y);

  void addPellet(Pellet pellet);
  void removePellet(Pellet pellet);

  bool update(PlayerCharacter pacman);
};
