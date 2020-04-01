#pragma once

struct Joystick {
  const uint8_t LEFT_PIN = 45;
  const uint8_t UP_PIN = 47;
  const uint8_t DOWN_PIN = 49;
  const uint8_t RIGHT_PIN = 51;

  Joystick() {
    pinMode(DOWN_PIN, INPUT);
    pinMode(UP_PIN, INPUT);
    pinMode(LEFT_PIN, INPUT);
    pinMode(RIGHT_PIN, INPUT);
  }

  uint8_t ReadInput() {
    uint8_t out = 0;
    bool left = digitalRead(LEFT_PIN);
    bool right = digitalRead(RIGHT_PIN);
    bool up = digitalRead(UP_PIN);
    bool down = digitalRead(DOWN_PIN);

    if (up == LOW) {
      out |= 1u;
    }
    if (down == LOW) {
      out |= 2u;
    }
    if (right == LOW) {
      out |= 4u;
    }
    if (left == LOW) {
      out |= 8u;
    }

    return out;
  }
};
