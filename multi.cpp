/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include "include/multi.h"

enum MsgType {
  ACK = 0,    // Your basic acknowledgement (because everyone deserves to be
              // acknowledged for their hardwork and good comm skills)
  INIT,       // Initialize communication message
  MAP_START,  // Map sending sequence start
  MAP_NODE,   // Map node
  MAP_END,    // Map sending sequence end
  STATE,      // Game state
  PLAYER      // Player location
};

MapPayload junctionToPayload(Junction* j) {
  MapPayload p;

  p.id = j->id;
  p.x = j->x;
  p.y = j->y;

  for (uint8_t i = 0; i < N_ORIENT; i++) {
    if (j->adjacent[i] != nullptr) {
      p.neighbours[i] = j->adjacent[i]->id;
    } else {
      p.neighbours[i] = INVALID_JID;
    }
  }

  return p;

}

void Device::sendGameState(StatePayload p) {
  buffer.send(STATE, &p, sizeof(StatePayload));
}

void Device::sendEntityLocation(PlayerPayload p) {
  buffer.send(PLAYER, &p, sizeof(PlayerPayload));
}

void Device::begin() {
  state = START;
  id = 0;

  timeout = 0;
  failures = 0;

  waitingForAck = false;

  buffer.begin();
}

void Device::end() { buffer.end(); }

ComState Device::getState() { return state; }

Device::Device(uint8_t port) : buffer(port) {}

Server::Server(uint8_t id) : Device(id) {}

Client::Client() : Device(1) {}

bool Device::checkForAck() {
  if (buffer.hasMessage()) {
    Message* msg = buffer.getMessage();

    // We want an ACK, nothing else...
    if (msg->type == ACK) {
      failures = 0;

      waitingForAck = false;

      delete msg;
      return true;
    }

  } else if (!checkTimeout()) {
    waitingForAck = false;
  }

  return false;
}

bool Device::checkTimeout() {
  if (millis() - timeout >= TIMEOUT_LENGTH) {
    failures++;
    if (failures > 3) {
      state = DISCONNECTED;
    }
    return false;
  }
  return true;
}

void Server::handle() {
  buffer.recieve();

  switch (state) {
    case START:
      if (!waitingForAck) {
        buffer.send(INIT, nullptr, 0);
        timeout = millis();
        waitingForAck = true;
      } else if (checkForAck()) {
        Serial.println("Transitioning to Map");
        beginMap();
      }
      break;
    case MAP:
      if (!waitingForAck) {
        if (index == 0) {
          buffer.send(MAP_START, &num_elements, 1);
        } else if (index <= num_elements) {
          buffer.send(
            MAP_NODE,
            &junctionToPayload(builder->copy_arr[index - 1]), // Bad but it gets copied so its fine
            sizeof(MapPayload)
          );
        } else {
          buffer.send(MAP_END, nullptr, 0);
        }
        timeout = millis();
        waitingForAck = true;
      } else if (checkForAck()) {
        if (index <= num_elements) {
          index++;
          Serial.print("Increase map send index: ");
          Serial.println(index);
        } else {
          Serial.println("Transitioning to Loop");
          beginLoop();
        }
      }
      break;
    case LOOP:
      if (buffer.hasMessage()) {
        Message* msg = buffer.getMessage();
        void* payload = new uint8_t[msg->size];
        if (msg->type == STATE) {
          sCallback((StatePayload*)(payload));
        } else if (msg->type == PLAYER) {
          pCallback((PlayerPayload*)(payload));
        }
        delete msg;
      }
      break;
    case DISCONNECTED:
    default:
      break;  // Do nothing (idle until reset) as we are disconnected or in a
              // bad state
  }
}

void Server::beginMap() {
  index = 0;
  num_elements = builder->junctionCount;

  state = MAP;
}

void Server::beginLoop() {
  delete builder;

  state = LOOP;
}


void Client::handle() {
  buffer.recieve();

  switch (state) {
    case START:
      if (buffer.hasMessage()) {
        Message* msg = buffer.getMessage();
        if (msg->type == INIT) {
          buffer.send(ACK, &id, 0);
          beginMap();
          Serial.println("Transitioning to Map");
        }
        delete msg;
      }
      break;
    case MAP:
      if (buffer.hasMessage()) {
        Message* msg = buffer.getMessage();
        if (msg->type == MAP_START) {
          buffer.send(ACK, nullptr, 0);
          builder->SetJunctionCount(*(uint8_t*)(msg->payload));
        } else if (msg->type == MAP_NODE) {
          MapPayload* load = (MapPayload*)(msg->payload);
          buffer.send(ACK, nullptr, 0);
          processMap(load);
        } else if (msg->type == MAP_END) {
          buffer.send(ACK, nullptr, 0);
          beginLoop();
        } else {
          delete msg;
          break; // Just force that we don't send back an ACK for an invalid msg
        }
        delete msg;
      }
      break;
    case LOOP:
      if (buffer.hasMessage()) {
        Message* msg = buffer.getMessage();
        if (msg->type == STATE) {
          sCallback((StatePayload*)(msg->payload));
        } else if (msg->type == PLAYER) {
          pCallback((PlayerPayload*)(msg->payload));
        }
        delete msg;
      }
      break;
    case DISCONNECTED:
    default:
      break;  // Do nothing (idle until reset) as we are disconnected or in a
              // bad state
  }
}

void Client::beginMap() {
  builder = new MapBuilder();

  state = MAP;
}

void Client::beginLoop() {
  state = LOOP;
}

void Client::processMap(MapPayload* m) {
  builder->SetJunctionLocations(m->id, m->x, m->y);
  for (uint8_t i = 0; i < N_ORIENT; i++) {
    if (m->neighbours[i] != INVALID_JID) {
      builder->LinkJunctions(m->id, m->neighbours[i]);
    }
  }
}
