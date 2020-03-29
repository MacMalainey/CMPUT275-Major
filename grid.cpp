/**
 *
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 *
 */

#include "include/grid.h"

#include <Arduino.h>

#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320

uint16_t randomCounter = 0;

///////////////////////////////////////////////////////////////////////////////
// Cell class
///////////////////////////////////////////////////////////////////////////////

Cell::Cell(uint16_t id) { this->id = id; }

uint16_t Cell::getID() { return this->id; }

///////////////////////////////////////////////////////////////////////////////
// Row class
///////////////////////////////////////////////////////////////////////////////

Row::Row(uint8_t divisions) {
  this->divisions = divisions;
  this->cells = new Cell *[divisions];

  for (uint8_t i = 0; i < divisions; i++) {
    this->cells[i] = new Cell(randomCounter++);
  }
}

Row::~Row() { delete[] cells; }

Cell *Row::getCell(uint8_t index) { return this->cells[index]; }

///////////////////////////////////////////////////////////////////////////////
// Grid class
///////////////////////////////////////////////////////////////////////////////

Grid::Grid() {
  divisions = 0;
  rows = nullptr;
  sizeX = DISPLAY_WIDTH;
  sizeY = DISPLAY_HEIGHT;
}

Grid::~Grid() { delete[] rows; }

Row *Grid::getRow(uint8_t index) { return this->rows[index]; }

uint8_t Grid::getRowIndex(uint16_t x) {
  uint8_t index = floor((divisions * x) / DISPLAY_WIDTH);
  return constrain(index, 0, divisions - 1);
}

uint8_t Grid::getCellIndex(uint16_t y) {
  uint8_t index = floor((divisions * y) / DISPLAY_HEIGHT);
  return constrain(index, 0, divisions - 1);
}

void Grid::addPellet(Pellet pellet) {
  uint8_t rowIndex = getRowIndex(pellet.x);
  uint8_t cellIndex = getCellIndex(pellet.y);

  // rows[rowIndex]->cells[cellIndex]
}

void Grid::Generate(uint8_t divisions) {
  this->divisions = divisions;
  this->rows = new Row *[divisions];

  for (uint8_t i = 0; i < divisions; i++) {
    this->rows[i] = new Row(divisions);
  }
}