

enum ComState {
  START,
  MAP_LOAD

};

class Device {
 protected:
  bool needAck;
  // uint64_t timeout;
};

class Server : Device {};

class Client : Device {};
