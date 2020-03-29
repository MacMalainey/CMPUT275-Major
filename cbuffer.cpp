
#include "include/comm.h"



class CommBuffer {

private:
    static const uint8_t MAX_READ = 30; // Maximum number of bytes to read per call to recieve
    static const uint8_t MAX_BUFFER_LENGTH = sizeof(uint8_t); // We should never send more than 256 bytes at once
    static const uint64_t MSG_TIMEOUT = 1000; // 1 second to timeout on a message

    static const uint8_t START_FLAG = 'S';

    static const uint8_t START_BYTE = 0;
    static const uint8_t SIZE_BYTE = 1;
    static const uint8_t TYPE_BYTE = 2;
    static const uint8_t CHECKSUM_BYTE = 3; // Technically bytes 3 and 4

    static const uint8_t HEADER_LENGTH = 5;

    HardwareSerial* serial;

    bool isWaiting;

    uint64_t timestamp;

    uint8_t bufferLen;

    uint8_t buffer[MAX_BUFFER_LENGTH];

    bool msgReady;

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
    uint8_t serialize(MsgType type, void* payload, uint8_t size, uint8_t* buffer) {
        uint16_t checksum = genChecksum(payload, size);
        buffer[START_BYTE] = START_FLAG; // Denote new message start
        buffer[SIZE_BYTE] = size; // The size of the message (not just meta data)
        buffer[TYPE_BYTE] = type; // What the message contains
        buffer[CHECKSUM_BYTE] = checksum % 256; // First 8 bits of checksum
        buffer[CHECKSUM_BYTE + 1] = checksum >> 8; // Last 8 bits of checksum
        memcpy(buffer + HEADER_LENGTH, payload, size);
        return size + HEADER_LENGTH;
    }

    uint16_t genChecksum(void* payload, uint8_t size) {
        uint16_t checksum = 0;
        uint8_t* bytes = static_cast<uint8_t*>(payload); // The compiler isn't happy if we use our original void*

        for (uint8_t i = 0; i < size; i++) {
            checksum += bytes[i];
        }

        return checksum;
    }

    /**
     * 
     * 
     * 
     * Note:
     * This function has a lot of C style stuff to it...
     * Don't touch unless you are comfortable with C memory allocation (talk to Mac otherwise)
     */
    Message* deserialize() {
        Message* msg = new Message();
        msg->type = buffer[TYPE_BYTE];
        msg->size = buffer[SIZE_BYTE];
        msg->payload = malloc(msg->size); // Cause "new void[SIZE];" doesn't work ;(
        memcpy(msg->payload, buffer + HEADER_LENGTH, msg->size);
        return msg;
    }

    bool validate() {
        uint16_t checksum = genChecksum(buffer + HEADER_LENGTH, buffer[SIZE_BYTE]);
        uint16_t valid = buffer[CHECKSUM_BYTE + 1];
        valid <<= 8;
        valid += buffer[CHECKSUM_BYTE];

        return checksum == valid;
    }

    void cleanBuffer() {
        uint8_t remain = bufferLen - (buffer[SIZE_BYTE] + HEADER_LENGTH);
        if (remain > 0 && buffer[bufferLen - remain] == START_FLAG) { // We seem to be recieving another message, check if true, otherwise just dump the whole buffer
            memcpy(buffer, buffer + (bufferLen - remain), remain);
            bufferLen = remain;
        } else {
            bufferLen = 0;
        }
    }

public:
    // Flag to determine behaviour on dropped messages
    // If true it will re-request any dropped messages
    // If false it will drop the message and continue to wait for new messages
    bool reqOnDrop;

    CommBuffer(uint8_t select) {
        bufferLen = 0;
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
                // TODO: We should throw an exception here
                break;
        }
    }

    void send(MsgType type, void* payload, uint8_t length) {
        uint8_t wBuffer[MAX_BUFFER_LENGTH];

        uint8_t size = serialize(type, payload, length, wBuffer);

        for (uint8_t i = 0; i < size; i++) {
            serial->write(wBuffer[i]);
        }

    }

    void recieve() {
        uint8_t bytes = 0;

        while(Serial.available() > 0 && bytes < MAX_READ) {
            if (isWaiting) {
                uint8_t byte = serial->read();
                if (byte == START_FLAG) {
                    isWaiting = false;  // Signify that we are actively reading a message
                    buffer[START_BYTE] = byte;
                }
                bytes++;

                timestamp = millis();
            } else {
                buffer[bufferLen + bytes] = serial->read();
                bytes++;
                if (!msgReady && bufferLen >= buffer[SIZE_BYTE] + HEADER_LENGTH) {
                    if (validate()) {
                        msgReady = true;
                    } else { // Our validate call failed and a message wasn't already prepared so we should drop what we currently have
                        cleanBuffer();
                    }
                }
            }
        }

        bufferLen += bytes;
    }

    bool hasMessage() {
        return msgReady;
    }

    Message* getMessage() {
        if (!msgReady) return NULL;
        msgReady = false;

        Message* msg = deserialize();
        cleanBuffer();

        return msg;
    }

};
