#include "include/multi.h"

enum MsgType {
  ACK = 0,        // Your basic acknowledgement (because everyone deserves to be
              // acknowledged)
  INIT,       // Initialize communication message
  MAP_START,  // Map sending sequence start
  MAP_NODE,   // Map node
  MAP_END,    // Map sending sequence end
  STATE,      // Game state
  PLAYER,      // Player location
  CONFIRM,    // Signals a request for a confirm (from server)
};

void Device::sendGameState(StatePayload p) {
  buffer.send(STATE, &p, sizeof(StatePayload));
}

void Device::sendEntityLocation(PlayerPayload p) {
  buffer.send(PLAYER, &p, sizeof(PlayerPayload));
}

void Device::begin() {
  state = START;
  index = 0;
  num_elements = 0;
  id = 0;

  timeout = 0;
  failures = 0;

  waitingForAck = false;

  buffer.begin();
}

void Device::end() {
  buffer.end();
}

Device::Device(uint8_t port) : buffer(port) {}

Server::Server(uint8_t id) : Device(id) {}

Client::Client() : Device(1) {}

bool Device::checkForAck() {
  if(buffer.hasMessage()) {
    Message* msg = buffer.getMessage();

    // We want an ACK, nothing else...
    if (msg->type == ACK) {
      failures = 0;

      delete msg;
      return true;
    }

  } else if (millis() - timeout >= TIMEOUT_LENGTH) {
    failures++;
    waitingForAck = false;
  }

  return false;
}

void Server::handle() {
  buffer.recieve();

  switch(state) {
    case START:
      if (!waitingForAck) {
        buffer.send(INIT, &id, 0);
        timeout = millis();
        waitingForAck = true;
      } else if (checkForAck()) {
        state = MAP;
        Serial.println("Transitioning to Map");
      }
      break;
    case MAP:
      break;
    case LOOP:
      if (buffer.hasMessage()) {
        Message* msg = buffer.getMessage();
        void* payload = new uint8_t[msg->size];
        if (msg->type == STATE) {
          sCallback(static_cast<StatePayload*>(payload));
        } else if (msg->type == PLAYER) {
          pCallback(static_cast<PlayerPayload*>(payload));
        }
        delete msg;
      }
      break;
    case DISCONNECTED:
    default:
      break; // Do nothing (idle until reset) as we are disconnected or in a bad state
  }
}

void Client::handle() {
  buffer.recieve();

  switch(state) {
    case START:
      if (buffer.hasMessage()) {
        Serial.println("Got a message");
        Message* msg = buffer.getMessage();
        if (msg->type == INIT) {
          buffer.send(ACK, &id, 0);
          state = MAP;
          Serial.println("Transitioning to Map");
        }
        delete msg;
      }
      break;
    case MAP:
      // TODO it might be best to build a generator class that I can input nodes into
      if (buffer.hasMessage()) {
        Serial.println("Got a message");
        Message* msg = buffer.getMessage();
        if (msg->type == MAP_START) {
          buffer.send(ACK, &id, 0);
        } else if (msg->type == MAP_NODE) {
          buffer.send(ACK, &id, 0);
        } else if (msg->type == MAP_END) {
          buffer.send(ACK, &id, 0);
          state = LOOP;
        }
        delete msg;
      }
      break;
    case LOOP:
      if (buffer.hasMessage()) {
        Message* msg = buffer.getMessage();
        void* payload = new uint8_t[msg->size];
        memcpy(payload, msg->payload, msg->size);
        if (msg->type == STATE) {
          sCallback(static_cast<StatePayload*>(payload));
        } else if (msg->type == PLAYER) {
          pCallback(static_cast<PlayerPayload*>(payload));
        }
        delete msg;
      }
      break;
    case DISCONNECTED:
    default:
      break; // Do nothing (idle until reset) as we are disconnected or in a bad state
  }
}