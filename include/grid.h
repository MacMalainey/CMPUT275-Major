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
  PlayerCharacter Ghosts;
  Pellet Pellets;
};

class Grid {
  friend class Cell;

 private:

  Point getGridPos(uint16_t x, uint16_t y);
  
 public:
  Grid();

  void addCharacter(PlayerCharacter character);
  void removeCharacter(PlayerCharacter character);
  void characterMoved(PlayerCharacter character);

  void addPellet(Pellet pellet);
  void removePellet(Pellet pellet);

  void update();
};
