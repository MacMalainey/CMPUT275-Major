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

#include "comm.h"
#include "map.h"
#include "misc.h"

enum ComState { START = 0, MAP, LOOP, DISCONNECTED };

// This is NOT how callbacks are supposed to work BUT
// C++ doesn't LIKE using member class function pointers for callbacks unless
// you specific class type...  WHYYYYYY (its ok I can work around)

struct PlayerCallback {
  PlayerPayload* load = nullptr;
};

struct StateCallback {
  StatePayload* load = nullptr;
};

class Device {
 protected:
  const uint32_t TIMEOUT_LENGTH = 1000UL;

  CommBuffer buffer;
  ComState state;

  uint8_t id;

  uint32_t timeout;
  uint8_t failures;

  bool waitingForAck;

  bool checkForAck();
  bool checkTimeout();

public:
  PlayerCallback* pCallback;
  StateCallback* sCallback;
  MapBuilder* builder;

  void sendGameState(StatePayload p);
  void sendEntityLocation(PlayerPayload p);
  ComState getState();

  Device(uint8_t port);

  void begin();
  void handle();
  void end();
};

class Server : public Device {
private:
  void beginMap();
  void beginLoop();

  // Used for iterating through the map
  uint8_t index;
  uint8_t num_elements;

public:

  void handle();
  Server(uint8_t id);
};

class Client : public Device {
private:
  void beginMap();
  void beginLoop();

  void processMap(MapPayload* m);

public:

  void handle();
  Client();
};
