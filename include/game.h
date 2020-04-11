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
#include "grid.h"
#include "input.h"
#include "map.h"
#include "multi.h"
#include "screen.h"

enum State { SETUP, WAIT_FOR_CONNECTION, READY, PACMAN_DEATH, GAME_END };

class ServerGame {
 public:
   /**
    * @brief Construct a new ServerGame:: ServerGame object
    *
    */
  ServerGame();


  /**
   * @brief Initial things that the server runs once before the game starts
   */
  void Start();

  /**
   * @brief The game loop for the server
   */
  void Loop();

 private:
  uint8_t current_lives = 3;
  uint16_t score = 0;
  uint8_t player_count = 0;

  /**
   * @brief Updates the score in the top left corner of screen
   */
  void updateScore();

  /**
   * @brief Draws the intial three pacman lives in the top right corner for the server
   *
   */
  void drawLives();


  /**
   * @brief Decrements the life counter in the top right corner of the screen
   *        depending on how many lives pacman currently has for the SERVER
   */
  void decrementLives();

  Map *map;
  uint16_t map_color;

  Grid grid;

  Screen screen;
  Joystick joy;
  Server devices[2];

  Pellet pellets[70];
  PlayerCharacter characters[3];

  PlayerCharacter &myChar = characters[0];

  State GameState;
  Point startingPoint;
};

class ClientGame {
 public:
   /**
    * @brief Construct a new ClientGame:: ClientGame object
    *
    */
  ClientGame();

  /**
   * @brief Initial things that the server runs once before the game starts
   */
  void Start();

  /**
   * @brief The game loop for the server
   */
  void Loop();

 private:

   /**
    * @brief Updates the score in the top left corner of screen
    */
   void updateScore();

   /**
    * @brief Draws the intial three pacman lives in the top right corner for the server
    *
    */
   void drawLives();


   /**
    * @brief Decrements the life counter in the top right corner of the screen
    *        depending on how many lives pacman currently has for the SERVER
    */
   void decrementLives();

  uint16_t distUntilDeadEnd(Point gLocation, Junction *junction,
                            Orientation orientation);
  void canSeePacman(PlayerCharacter ghost);

  State GameState;

  Screen screen;
  Joystick joy;
  Client device;

  uint8_t current_lives = 3;
  uint16_t score = 0;

  Map *map;
  uint16_t map_color;

  PlayerCharacter myChar;

  // characters[0] should be PacMan.
  PlayerCharacter characters[3];
};
