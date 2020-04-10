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

  virtual void Draw(Screen &screen) = 0;
  virtual void Clear(Screen &screen) = 0;

  Point location;
  uint16_t color;

  // Used for the LinkedList class for checking inequality
  bool operator!=(const Drawable &other) const;
};

struct Pellet : public Drawable {
  Pellet();
  Pellet(Point startPoint);

  void Draw(Screen &screen) final;
  void Clear(Screen &screen) final;

  bool isPowerUp = false;

  static void GeneratePellets(Vector<Pellet> &pellets, const Map *map);
};

struct PlayerCharacter : public Drawable {
  PlayerCharacter();
  PlayerCharacter(Point startPoint);

  bool isPacman = true;
  bool canSeePacman = false;

  Orientation orientation = N_ORIENT;
  Orientation nextDirection = N_ORIENT;
  Junction *currentJunction;

  void handleMovement(Screen &screen, uint8_t input, Map *map);
  void Move(Screen &screen, uint8_t speed = 1);

  bool isValidDirection(Orientation direction);
  void SetOrientation(uint8_t newOrientation);
  void checkTunnelDirection(Screen &screen, uint8_t input, Map *map,
                            Orientation direction, Orientation opposite);

  void MoveTunnel(uint8_t input, Screen &screen, Map *map);

  void Draw(Screen &screen) final;
  void Clear(Screen &screen) final;
  void DrawGhostBody(Screen &screen);

  uint16_t color;
};
