/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#include "include/comm.h"

uint8_t CommBuffer::serialize(uint8_t type, void* payload, uint8_t size,
                              uint8_t* buffer) {
  uint16_t checksum = genChecksum(payload, size);
  buffer[START_BYTE] = START_FLAG;  // Denote new message start
  buffer[SIZE_BYTE] = size;  // The size of the message (not just meta data)
  buffer[TYPE_BYTE] = type;  // What the message contains
  buffer[CHECKSUM_BYTE] = checksum % 256;     // First 8 bits of checksum
  buffer[CHECKSUM_BYTE + 1] = checksum >> 8;  // Last 8 bits of checksum
  memcpy(buffer + HEADER_LENGTH, payload, size);
  return size + HEADER_LENGTH;
}

uint16_t CommBuffer::genChecksum(void* payload, uint8_t size) {
  uint16_t checksum = 0;
  uint8_t* bytes = static_cast<uint8_t*>(
      payload);  // The compiler isn't happy if we use our original void*

  for (uint8_t i = 0; i < size; i++) {
    checksum += bytes[i];
  }

  return checksum;
}

Message* CommBuffer::deserialize() {
  Message* msg = new Message();
  msg->type = buffer[TYPE_BYTE];
  msg->size = buffer[SIZE_BYTE];
  msg->payload = new uint8_t[msg->size];
  memcpy(msg->payload, buffer + HEADER_LENGTH, msg->size);
  return msg;
}

bool CommBuffer::validate() {
  Serial.println("Running validate check");
  uint16_t checksum = genChecksum(buffer + HEADER_LENGTH, buffer[SIZE_BYTE]);
  uint16_t valid = buffer[CHECKSUM_BYTE + 1];
  valid <<= 8;
  valid += buffer[CHECKSUM_BYTE];

  return checksum == valid;
}

void CommBuffer::cleanBuffer() {
  bufferLen = 0; // This was a long function but I decided to change functionality
  isWaiting = true;
  msgReady = false;
}

CommBuffer::CommBuffer(uint8_t select) {  // This should be created as a factory
                                          // producing singletons
  bufferLen = 0;
  msgReady = false;
  isWaiting = true;
  reqOnDrop = true;

  // Select which buffer we should be using
  switch (select) {
    case 1:
      serial = &Serial1;
      break;
    case 2:
      serial = &Serial2;
      break;
    case 3:
      serial = &Serial3;
      break;
    default:
      break; // TODO: We should throw an exception here
  }
}

void CommBuffer::send(uint8_t type, void* payload, uint8_t length) {
  uint8_t wBuffer[length + HEADER_LENGTH];
  uint8_t size = serialize(type, payload, length, wBuffer);

  for (uint8_t i = 0; i < size; i++) {
    serial->write(wBuffer[i]);
    Serial.print(wBuffer[i]);
  }
  Serial.println("");
}

void CommBuffer::recieve() {
  uint8_t bytes = 0;

  while (serial->available() > 0 &&
         bytes < MAX_READ && !msgReady) {
    if (isWaiting) {
      uint8_t byte = serial->read();
      if (byte == START_FLAG) {
        isWaiting = false;  // Signify that we are actively reading a message
        buffer[START_BYTE] = byte;
        timestamp = millis();
        bytes++;
      }
    } else {
      buffer[bufferLen + bytes] = serial->read();
      Serial.print(buffer[bufferLen + bytes]);
      // Serial.print(bufferLen + bytes);
      bytes++;
      if (!msgReady && bufferLen + bytes >= buffer[SIZE_BYTE] + HEADER_LENGTH) {
        if (validate()) {
          msgReady = true;
          Serial.println("MSG READY");
        } else {  // Our validate call failed and a message wasn't already
                  // prepared so we should drop what we currently have
          cleanBuffer();
        }
      }
    }
  }

  bufferLen += bytes;
}

void CommBuffer::begin() { serial->begin(9600); }

void CommBuffer::end() { serial->end(); }

bool CommBuffer::hasMessage() { return msgReady; }

Message* CommBuffer::getMessage() {
  if (!msgReady) return nullptr;

  Message* msg = deserialize();
  cleanBuffer();

  return msg;
}

CommBuffer::~CommBuffer() {}
