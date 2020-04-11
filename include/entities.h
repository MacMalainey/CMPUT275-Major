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
  /**
   * @brief Construct a new Drawable:: Drawable object
   *
   */
  Drawable();
  Drawable(Point startPoint);

  virtual void Draw(Screen &screen) = 0;
  virtual void Clear(Screen &screen) = 0;

  // Properties that can get reused by base classes
  Point location;
  uint16_t color;

  /**
   * @brief Inequality operator for drawables. Pellets are guaranteed not to be
   *        in the same position.
   *
   * @param other The other drawable we're comparing to.
   * @return true If not equal.
   * @return false If equal.
   */
  bool operator!=(const Drawable &other) const;
};

struct Pellet : public Drawable {
  /**
   * @brief Construct a new Pellet:: Pellet object
   *
   */
  Pellet();
  Pellet(Point startPoint);

  /**
   * @brief Draws the pellet.
   *
   * @param screen The screen (drawing, etc.).
   */
  void Draw(Screen &screen) final;

  /**
   * @brief Removes the drawing of the player.
   *
   * @param screen The screen (drawing, etc.).
   */
  void Clear(Screen &screen) final;

  // For powerup pellet
  bool isPowerUp = false;

  /**
   * Originally written to generate pellets every 10 pixels between junctions
   * We didn't have enough SRAM to do that, so we opted to generate the pellets
   * just at the junctions. Code is similar.
   *
   * @param pellets Pointer to array of pellets where it should be stored
   * @param startJunction Junction to start generating at
   * @param junctionCount number of junctions to generate pellets for
   */
  static void GeneratePellets(Pellet *pellets, Junction *startJunction,
                              uint8_t junctionCount);
};

struct PlayerCharacter : public Drawable {
  /**
   * @brief Construct a new Player Character:: Player Character object
   *
   */
  PlayerCharacter();
  PlayerCharacter(Point startPoint);

  bool isPacman = true;
  bool canSeePacman = false;

  Orientation orientation = N_ORIENT;
  Orientation nextDirection = N_ORIENT;
  Junction *currentJunction;

  /**
   * @brief Run each game loop. Handles everything movement related.
   *
   * @param screen The screen (drawing, etc.).
   * @param input The direction of input.
   * @param map The map (junctions, etc.).
   */
  void handleMovement(Screen &screen, uint8_t input, Map *map);

  /**
   * @brief Moves the player by `speed` pixels in it's current orientation
   *        and redraws the character.
   *
   * @param screen The screen (drawing, etc.).
   * @param speed How quickly to move the player (defaults to defaultSpeed).
   */
  void Move(Screen &screen, uint8_t speed = 1);

  /**
   * @brief Checks if the player can move in the requested direction.
   *
   * @param direction The direction we're checking.
   * @return true If we can move in the given direction.
   * @return false If we can't move in the given direction.
   */
  bool isValidDirection(Orientation direction);

  /**
   * @brief Attempts to set the orientation of the player.
   *
   * @param newOrientation The direction the player is trying to move.
   */
  void SetOrientation(uint8_t newOrientation);

  /**
   * @brief Handles player movement when not in a tunnel.
   *
   * @param screen The screen (drawing, etc.).
   * @param input The direction of input.
   * @param map The map (junctions, etc.).
   * @param direction The current direction we're checking in.
   * @param opposite The opposite direction to the current direction.
   */
  void checkTunnelDirection(Screen &screen, uint8_t input, Map *map,
                            Orientation direction, Orientation opposite);

  /**
   * @brief Handles player movement in each of the four possible cardinal
   *        directions.
   *
   * @param input The direction of input.
   * @param screen The screen (drawing, etc.).
   * @param map he map (junctions, etc.).
   */  
  void MoveTunnel(uint8_t input, Screen &screen, Map *map);

  /**
   * @brief Draws the player. If the player is Pacman, it draws the correct
   *        sprite; otherwise it draws a ghost.
   *
   * @param screen The screen (drawing, etc.).
   */
  void Draw(Screen &screen) final;

  /**
   * @brief Draws the ghost sprite.
   *
   * @param screen The screen (drawing, etc.).
   */
  void DrawGhostBody(Screen &screen);

  /**
   * @brief Clears the character from the screen by drawing a black circle over
   *        the location.
   *
   * @param screen The screen (drawing, etc.).
   */
  void Clear(Screen &screen) final;


};
