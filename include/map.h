/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#pragma once

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

#include "misc.h"

// This is our "Node" class for our Map graph
class Junction {
 public:
  // Cords
  uint16_t x;
  uint16_t y;

  // Array of linked nodes (follows compass orientation struct)
  Junction **adjacent;
  uint8_t id;  // Used for hashing and sending data over serial

  /**
   * Description:
   * Junction constructor
   *
   * Parameters:
   * x (uint16_t): x cord
   * y (uint16_t): y cord
   */
  Junction(uint16_t x, uint16_t y);

  /**
   * Description:
   * Get the next junction in the provided direction
   *
   * Parameters:
   * d (Orientation): direction of node
   *
   * Returns:
   * junction (Junction*): pointer to junction object, null if no junction in that direction
   */
  Junction *next(Orientation d);

  /**
   * Description:
   * Link another junction to this one (links both ways)
   *
   * Parameters:
   * j (Junction*): junction to link
   *
   * Returns:
   * direction (Orientation): direction it was linked to (N_ORIENT if they are diagonal from each other)
   */
  Orientation link(Junction *j);
};

class Map {
  friend struct MapBuilder;

 private:
  // All the nodes
  Junction **nodes = nullptr;
  // Number of nodes
  uint8_t n = 0;

 public:

  /**
   * Description:
   * Constructs the Map
   *
   * Parameters:
   * nodes (Junction**): array to all junction objects
   * n (uint8_t): number of nodes
   */
  Map(Junction **nodes, uint8_t n);
  ~Map();

  /**
   * Description:
   * Gets cordinates of a junction
   *
   * Parameters:
   * startingNode (Junction*): node to get cords of
   *
   * Returns:
   * cords (Point): cords of given junction
   */
  Point getXY(Junction *startingNode);
  /**
   * Description:
   * Return first junction
   *
   * Parameters:
   * startingNode (Junction*): node to get cords of
   *
   * Returns:
   * node (Junction*): first junction
   */
  Junction *GetStart();
  /**
   * Description:
   * Returns number of nodes
   *
   * Returns:
   * n (uint8_T): number of nodes
   */
  uint8_t GetNodeCount();
};

struct MapBuilder {
  // Array of all junctions
  Junction **copy_arr;
  // All links that need to be made
  Point *links;

  // Number of junctions
  uint8_t junctionCount = 0;
  // Number of links
  uint16_t linkCount = 0;

  /**
   * Description:
   * Initializes builder for given number of nodes
   *
   * Parameters:
   * count (uint8_t): number of nodes
   */
  void SetJunctionCount(uint8_t count);

  /**
   * Description:
   * Sets the cords of a given junction
   *
   * Parameters:
   * id (uint8_t): junction to set cords of
   * x (uint8_t): x cord
   * y (uint8_t): y cord
   *
   */
  void SetJunctionLocations(uint8_t id, uint16_t x, uint16_t y);

  /**
   * Description:
   * Links two junctions at build
   *
   * Parameters:
   * id1, id2 (uint8_t): ids of junctions to link
   */
  void LinkJunctions(uint8_t junctionID1, uint8_t junctionID2);

  /**
   * Description:
   * Sets up a test map to be prepared for build
   */
  void TestGen();

  /**
   * Description:
   * Builds nodes
   *
   * Returns:
   * map (Map*): built map
   */
  Map *Build();

  /**
   * Description:
   * Extracts junction information from given map
   *
   * Parameters:
   * map (Map*): built map
   */
  void Debuild(Map *map);
};