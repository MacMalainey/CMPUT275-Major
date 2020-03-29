#pragma once

#include <Arduino.h>

enum MsgType {
  ACK,        // Your basic acknowledgement (because everyone deserves to be
              // acknowledged)
  INIT,       // Initialize communication message
  MAP_START,  // Map sending sequence start
  MAP_NODE,   // Map node
  MAP_END,    // Map sending sequence end
  STATE,      // Game state
  P_LOC,      // Player location
  CONFIRM,    // Signals a request for a confirm (from server)
};

struct Message {
  MsgType type;
  void* payload;  // Keep watch on this.  It might cause a memory leak...
  uint8_t size;

  ~Message() {
    free(payload);  // Because it is a void* we need to use free() ;(
  }
};
