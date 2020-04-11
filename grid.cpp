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

/**
 * @brief Construct a new Cell:: Cell object
 * 
 * @param id The number of cells so far.
 */
Cell::Cell(uint16_t id) { this->id = id; }

/**
 * @brief returns the ID of the cell.
 * 
 * @return uint16_t The ID of the cell.
 */
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

/**
 * @brief Destroy the Row:: Row object
 * 
 */
Row::~Row() { delete[] cells; }

/**
 * @brief Given the index, return the respective cell in the row.
 * 
 * @param index Index of the cell we're interested in.
 * @return Cell* The cell we're looking for.
 */
Cell *Row::getCell(uint8_t index) { return this->cells[index]; }

///////////////////////////////////////////////////////////////////////////////
// Grid class
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct a new Grid:: Grid object
 * 
 */
Grid::Grid() {
  divisions = 0;
  rows = nullptr;
  sizeX = DISPLAY_WIDTH;
  sizeY = DISPLAY_HEIGHT;
}

/**
 * @brief Destroy the Grid:: Grid object
 * 
 */
Grid::~Grid() { delete[] rows; }

/**
 * @brief 
 * 
 * @param index 
 * @return Row* 
 */
Row *Grid::getRow(uint8_t index) { return this->rows[index]; }

/**
 * @brief 
 * 
 * @param x 
 * @return uint8_t 
 */
uint8_t Grid::getRowIndex(uint16_t x) {
  uint8_t index = floor((divisions * x) / DISPLAY_WIDTH);
  return constrain(index, 0, divisions - 1);
}

/**
 * @brief 
 * 
 * @param y 
 * @return uint8_t 
 */
uint8_t Grid::getCellIndex(uint16_t y) {
  uint8_t index = floor((divisions * y) / DISPLAY_HEIGHT);
  return constrain(index, 0, divisions - 1);
}

/**
 * @brief 
 * 
 * @param pellet 
 */
void Grid::addPellet(Pellet pellet) {
  uint8_t rowIndex = getRowIndex(pellet.location.x);
  uint8_t cellIndex = getCellIndex(pellet.location.y);

  rows[rowIndex]->cells[cellIndex]->pellets.insert(pellet);
}

/**
 * @brief 
 * 
 * @param pellet 
 */
void Grid::removePellet(Pellet pellet) {
  uint8_t rowIndex = getRowIndex(pellet.location.x);
  uint8_t cellIndex = getCellIndex(pellet.location.y);

  rows[rowIndex]->cells[cellIndex]->pellets.remove(pellet);
}

/**
 * @brief 
 * 
 * @param divisions 
 */
void Grid::Generate(uint8_t divisions) {
  this->divisions = divisions;
  this->rows = new Row *[divisions];

  for (uint8_t i = 0; i < divisions; i++) {
    this->rows[i] = new Row(divisions, num_cells);
    num_cells += divisions;
  }
}

/**
 * @brief Iterates through the pellets in the current grid and checks if pacman
 *        is close.
 *
 * @param pacman The player.
 * @return true if there was a collision.
 * @return false if there wasn't a collision.
 */
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

/**
 * @brief Redraws pellets close to the ghost in the given cell.
 * 
 * @param screen The screen (drawing, etc.).
 * @param ghost The ghost.
 * @param current The first item in the cell. We iterate through this.
 */
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

/**
 * @brief Redraw pellets in the nearby cells a ghost is in (ghosts might draw 
 *        over pellets, so we need to redraw those pellets). 
 * 
 * @param screen The screen (drawing, etc.).
 * @param ghost The ghost.
 */
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
