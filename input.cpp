#include "include/input.h"

Joystick::Joystick() {
  pinMode(Joystick::DOWN_PIN, INPUT);
  pinMode(Joystick::UP_PIN, INPUT);
  pinMode(Joystick::LEFT_PIN, INPUT);
  pinMode(Joystick::RIGHT_PIN, INPUT);
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