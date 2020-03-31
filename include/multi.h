#pragma once

#include <Arduino.h>

#include "comm.h"

enum ComState {
  START,
  MAP,
  DEVICES,
  GAME,
  DISCONNECTED
};

struct StatePayload {
    uint8_t state;
};

struct PlayerPayload {
    uint8_t id;
    uint16_t x;
    uint16_t y;
};

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
