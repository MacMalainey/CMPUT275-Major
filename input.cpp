/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include "include/input.h"

Joystick::Joystick() {
  pinMode(Joystick::DOWN_PIN, INPUT_PULLUP);
  pinMode(Joystick::UP_PIN, INPUT_PULLUP);
  pinMode(Joystick::LEFT_PIN, INPUT_PULLUP);
  pinMode(Joystick::RIGHT_PIN, INPUT_PULLUP);
}

Orientation Joystick::ReadInput() {
  bool left = digitalRead(Joystick::LEFT_PIN);
  bool right = digitalRead(Joystick::RIGHT_PIN);
  bool up = digitalRead(Joystick::UP_PIN);
  bool down = digitalRead(Joystick::DOWN_PIN);

  if (up == LOW) {
    return Orientation::NORTH;
  }
  if (down == LOW) {
    return Orientation::SOUTH;
  }
  if (right == LOW) {
    return Orientation::EAST;
  }
  if (left == LOW) {
    return Orientation::WEST;
  }

  return Orientation::N_ORIENT;
}