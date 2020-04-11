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
   */
  void drawLives();


  /**
   * @brief Decrements the life counter in the top right corner of the screen
   *        depending on how many lives pacman currently has for the SERVER
   */
  void decrementLives();

  Map *map; // The map (contains junctions, etc.)
  uint16_t map_color; // The color of the map (random color chosen at beginning)

  Grid grid; // Spatial partitioning grid for pellet collisions

  Screen screen; // The TFT display (used for drawing, etc.)
  Joystick joy; // The input system (up, down, etc.)
  Server devices[2]; 

  Pellet pellets[70]; // Array that stores all pellets in the game
  PlayerCharacter characters[3]; // Array that stores all the players

  PlayerCharacter &myChar = characters[0]; // Should be Pacman

  State GameState; // Current state of the game
  Point startingPoint; // Spawn point for Pacman
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

  /**
   * @brief Given the junction a character is at and a direction to search in, 
   *        finds the maximum distance until a dead end in that direction.
   * 
   * @param gLocation The location of the character.
   * @param junction  The junction the character is at.
   * @param orientation The direction to search in.
   * @return uint16_t The Manhattan distance from the player to the dead end.
   */
  uint16_t distUntilDeadEnd(Point gLocation, Junction *junction,
                            Orientation orientation);

  /**
   * @brief Checks if Pacman is in the line of sight of a ghost. 
   * 
   * @param ghost The ghost we're checking line of sight for.
   */
  void canSeePacman(PlayerCharacter ghost);

  State GameState; // Current state of the game

  Screen screen; // The TFT display (used for drawing, etc.)
  Joystick joy; // The input system (up, down, etc.)
  Client device;

  uint8_t current_lives = 3;
  uint16_t score = 0;

  Map *map; // The map (contains junctions, etc.)
  uint16_t map_color; // The color of the map (random color chosen at beginning)

  PlayerCharacter myChar; // Should be a ghost

  // characters[0] should be PacMan.
  PlayerCharacter characters[3];
};
