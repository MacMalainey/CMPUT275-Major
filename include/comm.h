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

struct Message {
  uint8_t type;
  void* payload;  // Keep watch on this.  It might cause a memory leak...
  uint8_t size;

  ~Message() {
    free(payload);  // Because it is a void* we need to use free() ;(
  }
};

class CommBuffer {
 private:
  static const uint8_t MAX_READ =
      40;  // Maximum number of bytes to read per call to recieve
  static const uint8_t MAX_BUFFER_LENGTH =
      126;  // We should never send more than 100 bytes in one message

  static const uint8_t START_FLAG = 'S';

  static const uint8_t START_BYTE = 0;
  static const uint8_t SIZE_BYTE = 1;
  static const uint8_t TYPE_BYTE = 2;
  static const uint8_t CHECKSUM_BYTE = 3;  // Technically bytes 3 and 4

  static const uint8_t HEADER_LENGTH = 5;

  HardwareSerial* serial;

  bool isWaiting;
  bool msgReady;

  uint8_t bufferLen;

  uint8_t randomChar;

  uint8_t buffer[MAX_BUFFER_LENGTH];

  /**
   * Description:
   * Serializes the message to be sent over serial
   *
   * Serialized Payload Structure:
   * Byte 0: Char S => Denote message start
   * Byte 1: # of bytes in payload
   * Byte 2: Type of message
   * Byte 3 and 4: 16 bit checksum
   * Byte 5 to n: Payload
   *
   * Parameters:
   * type (uint8_t): Type of message
   * payload (void *): Message information
   * size (uint8_t): size of payload
   * buffer(uint8_t*): buffer to write to
   */
  uint8_t serialize(uint8_t type, void* payload, uint8_t size, uint8_t* buffer);


  /**
   * Description:
   * Generates a checksum for the given payload
   *
   * Parameters:
   * payload (void*): bytes to gen checksum for
   * size (uint8_t): size of payload
   */
  uint16_t genChecksum(void* payload, uint8_t size);

  /**
   * Description:
   * Deserializes the current contents of the buffer into a message
   *
   * Returns:
   * msg (Message*): Pointer to deserialized message
   */
  Message* deserialize();

  /**
   * Description:
   * Validates the current message in the buffer
   *
   * Returns:
   * isValid (bool): Whether or not the message is valid
   */
  bool validate();

  /**
   * Description:
   * Cleans the buffer (resets the pointer back to the start)
   */
  void cleanBuffer();

 public:

  /**
   * Description:
   * Communications Buffer Constructor
   *
   * Parameters:
   * select (uint8_t): Port select
   */
  CommBuffer(uint8_t select);
  ~CommBuffer();

  /**
   * Description:
   * Sends a message with the given data and meta data
   *
   * Parameters:
   * type (uint8_t): Type of message (for deseriatlization)
   * payload (void*): Data to get sent
   * length (uint8_t): Length of payload
   */
  void send(uint8_t type, void* payload, uint8_t length);


  /**
   * Description:
   * Recieves and processes any incoming bytes
   */
  void recieve();

  /**
   * Description:
   * Start the buffer
   */
  void begin();

  /**
   * Description:
   * Close the line and buffer
   */
  void end();

  /**
   * Description:
   * Returns a flag that shows if a complete message is in the buffer
   *
   * Returns:
   * hasMessage (bool): true if there is a complete message in the buffer
   */
  bool hasMessage();

  /**
   * Description:
   * Returns a flag that shows if a complete message is in the buffer
   *
   * Returns:
   * msg (Message*): Pointer to serialized msg obj (MUST BE FREED )
   */
  Message* getMessage();
};
