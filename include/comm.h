#include <Arduino.h>

enum MsgType {
    ACK, // Your basic acknowledgement (because everyone deserves to be acknowledged)
    INIT, // Initialize communication message
    MAP_START, // Map sending sequence start
    MAP_NODE, // Map node
    MAP_END, // Map sending sequence end
    STATE, // Game state
    P_LOC, // Player location
    CONFIRM, // Signals a request for a confirm (from server)
};

struct Message {

    MsgType type;
    void* payload; // Keep watch on this.  It might cause a memory leak...
    uint8_t size;

    ~Message() {
        free(payload); // Because it is a void* we need to use free() ;(
    }

};

class CommBuffer {

private:
    static const uint8_t MAX_READ = 30; // Maximum number of bytes to read per call to recieve
    static const uint8_t MAX_BUFFER_LENGTH = 255; // We should never send more than 256 bytes at once
    static const uint64_t MSG_TIMEOUT = 1000; // 1 second to timeout on a message

    static const uint8_t START_FLAG = 'S';

    static const uint8_t START_BYTE = 0;
    static const uint8_t SIZE_BYTE = 1;
    static const uint8_t TYPE_BYTE = 2;
    static const uint8_t CHECKSUM_BYTE = 3; // Technically bytes 3 and 4

    static const uint8_t HEADER_LENGTH = 5;

    HardwareSerial* serial;

    bool isWaiting;
    bool msgReady;

    uint64_t timestamp;

    uint8_t bufferLen;

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
     */
    uint8_t serialize(MsgType type, void* payload, uint8_t size, uint8_t* buffer);

    uint16_t genChecksum(void* payload, uint8_t size);

    /**
     * 
     * 
     * 
     * Note:
     * This function has a lot of C style stuff to it...
     * Don't touch unless you are comfortable with C memory allocation (talk to Mac otherwise)
     */
    Message* deserialize();

    bool validate();

    void cleanBuffer();

public:
    // Flag to determine behaviour on dropped messages
    // If true it will re-request any dropped messages
    // If false it will drop the message and continue to wait for new messages
    bool reqOnDrop;

    CommBuffer(uint8_t select);
    ~CommBuffer();

    void send(MsgType type, void* payload, uint8_t length);

    void recieve();

    bool hasMessage();

    Message* getMessage();

};

