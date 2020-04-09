/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#pragma once

#include <stdint.h>

#include "misc.h"

struct Joystick {
  const uint8_t LEFT_PIN = 45;
  const uint8_t UP_PIN = 47;
  const uint8_t DOWN_PIN = 49;
  const uint8_t RIGHT_PIN = 51;

  Joystick();

  Orientation ReadInput();
};
