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

  /**
   * @brief Construct a new Cell:: Cell object
   * 
   * @param id The number of cells so far.
   */
  Cell(uint16_t id);

 public:
  /**
   * @brief returns the ID of the cell.
   * 
   * @return uint16_t The ID of the cell.
   */
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

  /**
   * @brief Construct a new Row:: Row object
   * 
   * @param divisions The number of divisions for the grid.
   * @param num_cells The number of cells in the grid so far.
   */
  Row(uint8_t divisions, uint16_t num_cells);

  /**
   * @brief Destroy the Row:: Row object
   * 
   */
  ~Row();
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
  /**
   * @brief Construct a new Grid:: Grid object
   */
  Grid();

  /**
   * @brief Destroy the Grid object
   */
  ~Grid();

  uint8_t divisions; // The number of divisions.
  uint16_t num_cells = 0; // The number of cells in the grid so far.

  /**
   * @brief Generates the grid with the desired number of rows and columns.
   * 
   * @param divisions The number of divisions.
   */
  void Generate(uint8_t divisions);

  /**
   * @brief Gets the index of a row based on x.
   * 
   * @param x The x coordinate.
   * @return uint8_t The index of the row.
   */
  uint8_t getRowIndex(uint16_t x);

  /**
   * @brief Gets the index of a cell based on y.
   * 
   * @param y The y coordinate.
   * @return uint8_t The index of the cell.
   */
  uint8_t getCellIndex(uint16_t y);

  /**
   * @brief Adds a pellet to the grid.
   * 
   * @param pellet The pellet to add.
   */
  void addPellet(Pellet pellet);

  /**
   * @brief Removes a pellet from the grid.
   * 
   * @param pellet The pellet to remove.
   */
  void removePellet(Pellet pellet);

  /**
   * @brief Iterates through the pellets in the current grid and checks 
   *        if pacman is close.
   *
   * @param pacman The player.
   * @return true if there was a collision.
   * @return false if there wasn't a collision.
   */
  bool update(PlayerCharacter pacman);

  /**
   * @brief Redraws pellets close to the ghost in the given cell.
   * 
   * @param screen The screen (drawing, etc.).
   * @param ghost The ghost.
   * @param current The first item in the cell. We iterate through this.
   */
  void redrawClosePelletsInCell(Screen &screen, PlayerCharacter ghost,
                                LNode<Pellet> *current);

  /**
   * @brief Redraw pellets in the nearby cells a ghost is in (ghosts might 
   *        draw over pellets, so we need to redraw those pellets). 
   * 
   * @param screen The screen (drawing, etc.).
   * @param ghost The ghost.
   */
  void redrawPellets(Screen &screen, PlayerCharacter ghost);
};
