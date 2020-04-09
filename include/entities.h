/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#pragma once

#include <MCUFRIEND_kbv.h>
#include <stdint.h>

#include "misc.h"
#include "screen.h"

class Drawable {
 public:
  Drawable();
  Drawable(Point startPoint);
  void Move(Screen &screen);

  virtual void Draw(Screen &screen) = 0;
  virtual void Clear(Screen &screen) = 0;

  void SetOrientation(Orientation &newOrientation);

  Point location;
  uint16_t color;

  // Used for the LinkedList class for checking inequality
  bool operator!=(const Drawable &other) const;

  Orientation orientation = Orientation::EAST;
};

struct Pellet : public Drawable {
  Pellet();
  Pellet(Point startPoint);

  void Draw(Screen &screen) final;
  void Clear(Screen &screen) final;

  bool isPowerUp = false;
};

struct PlayerCharacter : public Drawable {
  PlayerCharacter();
  PlayerCharacter(Point startPoint);

  void Draw(Screen &screen) final;
  void Clear(Screen &screen) final;
  void DrawGhostBody(Screen &screen);

  bool isPacman = true;
};
