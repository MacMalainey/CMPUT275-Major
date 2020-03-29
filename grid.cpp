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

uint16_t randomCounter = 0;

///////////////////////////////////////////////////////////////////////////////
// Cell class
///////////////////////////////////////////////////////////////////////////////

Cell::Cell(uint16_t id) {
    this->id = id;
}

uint16_t Cell::getID() {
    return this->id;
}

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

Row::~Row() {
    delete[] cells;
}

Cell *Row::getCell(uint8_t index) {
    return this->cells[index];
}

///////////////////////////////////////////////////////////////////////////////
// Grid class
///////////////////////////////////////////////////////////////////////////////

Grid::Grid(uint8_t divisions) {
    this->divisions = divisions;
    this->rows = new Row *[divisions];

    for (uint8_t i = 0; i < divisions; i++) {
        this->rows[i] = new Row(divisions);
    }
}

Grid::~Grid() {
    delete[] rows;
}

Row *Grid::getRow(uint8_t index) {
    return this->rows[index];
}

Grid *testGrid(uint8_t divisions) {
    return new Grid(divisions);
}
