#pragma once

#include <Arduino.h>

#include "comm.h"

enum ComState { START = 0, MAP, LOOP, DISCONNECTED };

struct StatePayload {
  uint8_t state;
};

struct PlayerPayload {
  uint8_t id;
  uint16_t x;
  uint16_t y;
};

struct MapPayload {
  uint8_t id;             // Guaranteed to be > 0;
  uint8_t neighbours[4];  // TODO: Don't hardcode this value
  uint16_t x, y;
};

typedef void (*playerCb)(PlayerPayload*);
typedef void (*stateCb)(StatePayload*);

class Device {
 protected:
  const uint32_t TIMEOUT_LENGTH = 1000UL;

  CommBuffer buffer;
  ComState state;

  uint8_t index;
  uint8_t num_elements;

  uint8_t id;

  uint32_t timeout;
  uint8_t failures;

  bool waitingForAck;

  bool checkForAck();

 public:
  playerCb pCallback;
  stateCb sCallback;

  void sendGameState(StatePayload p);
  void sendEntityLocation(PlayerPayload p);

  Device(uint8_t port);

  virtual void begin();
  virtual void handle();
  virtual void end();
};

class Server : public Device {
 public:
  void handle() final;
  Server(uint8_t id);
};

class Client : public Device {
 private:
 public:
  void handle() final;
  Client();
};
