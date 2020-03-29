/**
 *
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 *
 */

#include <Arduino.h>

#include "include/game.h"

#define RAND_SEED_LENGTH 32

void setup() {
    init();
    Serial.begin(9600);

    uint32_t seed = 0;

    //  Generate a new random seed at start (gotta keep things *fresh*)
    for (int i = 0; i < RAND_SEED_LENGTH; i++) {
        seed |= (analogRead(A11) & 1) << i;
    }

    randomSeed(seed);
}

int main() {
    setup();

    Game game;
    game.Start();

    while (true) {
        game.Loop();
    }

    Serial.end();

    return 0;
}
