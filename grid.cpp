/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include "include/grid.h"

#include <Arduino.h>

#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320

///////////////////////////////////////////////////////////////////////////////
// Cell class
///////////////////////////////////////////////////////////////////////////////

Cell::Cell(uint16_t id) { this->id = id; }

uint16_t Cell::getID() { return this->id; }

///////////////////////////////////////////////////////////////////////////////
// Row class
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct a new Row:: Row object
 *
 * @param divisions The number of divisions for the grid.
 * @param num_cells The number of cells in the grid so far.
 */
Row::Row(uint8_t divisions, uint16_t num_cells) {
  this->divisions = divisions;
  this->cells = new Cell *[divisions];

  for (uint8_t i = 0; i < divisions; i++) {
    this->cells[i] = new Cell(num_cells++);
  }
}

Row::~Row() { delete[] cells; }

///////////////////////////////////////////////////////////////////////////////
// Grid class
///////////////////////////////////////////////////////////////////////////////

Grid::Grid() {
  divisions = 0;
  rows = nullptr;
}

Grid::~Grid() { delete[] rows; }

uint8_t Grid::getRowIndex(uint16_t x) {
  uint8_t index = floor((divisions * x) / DISPLAY_WIDTH);
  return constrain(index, 0, divisions - 1);
}

uint8_t Grid::getCellIndex(uint16_t y) {
  uint8_t index = floor((divisions * y) / DISPLAY_HEIGHT);
  return constrain(index, 0, divisions - 1);
}

void Grid::addPellet(Pellet pellet) {
  uint8_t rowIndex = getRowIndex(pellet.location.x);
  uint8_t cellIndex = getCellIndex(pellet.location.y);

  rows[rowIndex]->cells[cellIndex]->pellets.insert(pellet);
}

void Grid::removePellet(Pellet pellet) {
  uint8_t rowIndex = getRowIndex(pellet.location.x);
  uint8_t cellIndex = getCellIndex(pellet.location.y);

  rows[rowIndex]->cells[cellIndex]->pellets.remove(pellet);
}

void Grid::Generate(uint8_t divisions) {
  this->divisions = divisions;
  this->rows = new Row *[divisions];

  for (uint8_t i = 0; i < divisions; i++) {
    this->rows[i] = new Row(divisions, num_cells);
    num_cells += divisions;
  }
}

bool Grid::update(PlayerCharacter pacman) {
  uint8_t rowIndex = getRowIndex(pacman.location.x);
  uint8_t cellIndex = getCellIndex(pacman.location.y);

  LNode<Pellet> *current = rows[rowIndex]->cells[cellIndex]->pellets.getFront();
  Pellet pellet;

  while (current != nullptr) {
    pellet = current->item;

    if (pellet.location - pacman.location <= 4) {
      // remove the pellet from the grid
      removePellet(pellet);

      return true;
    }

    current = current->next;
  }

  return false;
}

void Grid::redrawClosePelletsInCell(Screen &screen, PlayerCharacter ghost,
                                    LNode<Pellet> *current) {
  Pellet pellet;

  while (current != nullptr) {
    pellet = current->item;

    if (pellet.location - ghost.location <= 10) {
      // redraw the pellet
      pellet.Draw(screen);
    }

    current = current->next;
  }
}

void Grid::redrawPellets(Screen &screen, PlayerCharacter ghost) {
  uint8_t rowIndex = getRowIndex(ghost.location.x);
  uint8_t cellIndex = getCellIndex(ghost.location.y);

  // Check first grid
  LNode<Pellet> *current = rows[rowIndex]->cells[cellIndex]->pellets.getFront();
  redrawClosePelletsInCell(screen, ghost, current);

  // Check the eight adjacent grids (horizontals, verticals, and diagonals)

  // LEFT
  if (rowIndex > 0) {
    current = rows[rowIndex - 1]->cells[cellIndex]->pellets.getFront();
    redrawClosePelletsInCell(screen, ghost, current);
  }
  // RIGHT
  if (rowIndex < divisions - 1) {
    current = rows[rowIndex + 1]->cells[cellIndex]->pellets.getFront();
    redrawClosePelletsInCell(screen, ghost, current);
  }
  // DOWN
  if (cellIndex > 0) {
    current = rows[rowIndex]->cells[cellIndex - 1]->pellets.getFront();
    redrawClosePelletsInCell(screen, ghost, current);
  }
  // UP
  if (cellIndex < divisions - 1) {
    current = rows[rowIndex]->cells[cellIndex + 1]->pellets.getFront();
    redrawClosePelletsInCell(screen, ghost, current);
  }
  // LEFT & UP
  if (rowIndex > 0 && cellIndex > 0) {
    current = rows[rowIndex - 1]->cells[cellIndex - 1]->pellets.getFront();
    redrawClosePelletsInCell(screen, ghost, current);
  }
  // RIGHT & DOWN
  if (rowIndex < divisions - 1 && cellIndex < divisions - 1) {
    current = rows[rowIndex + 1]->cells[cellIndex + 1]->pellets.getFront();
    redrawClosePelletsInCell(screen, ghost, current);
  }
  // RIGHT & UP
  if (rowIndex < divisions - 1 && cellIndex > 0) {
    current = rows[rowIndex + 1]->cells[cellIndex - 1]->pellets.getFront();
    redrawClosePelletsInCell(screen, ghost, current);
  }
  // LEFT & DOWN
  if (rowIndex > 0 && cellIndex < divisions - 1) {
    current = rows[rowIndex - 1]->cells[cellIndex + 1]->pellets.getFront();
    redrawClosePelletsInCell(screen, ghost, current);
  }
}
