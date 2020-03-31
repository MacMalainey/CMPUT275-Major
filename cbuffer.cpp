#include "include/comm.h"

uint8_t CommBuffer::serialize(MsgType type, void* payload, uint8_t size, uint8_t* buffer) {
    uint16_t checksum = genChecksum(payload, size);
    buffer[START_BYTE] = START_FLAG; // Denote new message start
    buffer[SIZE_BYTE] = size; // The size of the message (not just meta data)
    buffer[TYPE_BYTE] = type; // What the message contains
    buffer[CHECKSUM_BYTE] = checksum % 256; // First 8 bits of checksum
    buffer[CHECKSUM_BYTE + 1] = checksum >> 8; // Last 8 bits of checksum
    memcpy(buffer + HEADER_LENGTH, payload, size);
    return size + HEADER_LENGTH;
}

uint16_t CommBuffer::genChecksum(void* payload, uint8_t size) {
    uint16_t checksum = 0;
    uint8_t* bytes = static_cast<uint8_t*>(payload); // The compiler isn't happy if we use our original void*

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
    uint16_t checksum = genChecksum(buffer + HEADER_LENGTH, buffer[SIZE_BYTE]);
    uint16_t valid = buffer[CHECKSUM_BYTE + 1];
    valid <<= 8;
    valid += buffer[CHECKSUM_BYTE];

    return checksum == valid;
}

void CommBuffer::cleanBuffer() {
    uint8_t remain = bufferLen - (buffer[SIZE_BYTE] + HEADER_LENGTH);
    if (remain > 0 && buffer[bufferLen - remain] == START_FLAG) { // We seem to be recieving another message, check if true, otherwise just dump the whole buffer
        memcpy(buffer, buffer + (bufferLen - remain), remain);
        bufferLen = remain;
    } else {
        bufferLen = 0;
    }
}

CommBuffer::CommBuffer(uint8_t select) {  // This should be created as a factory
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
            return; // TODO: We should throw an exception here
            break;
    }

    serial->begin(9600);
}

void CommBuffer::send(MsgType type, void* payload, uint8_t length) {
    uint8_t wBuffer[length + HEADER_LENGTH];
    uint8_t size = serialize(type, payload, length, wBuffer);

    for (uint8_t i = 0; i < size; i++) {
        serial->write(wBuffer[i]);
    }

}

void CommBuffer::recieve() {
    uint8_t bytes = 0;

    while(serial->available() > 0 && bytes < MAX_READ) { // TODO check for buffer overflow
        if (isWaiting) {
            uint8_t byte = serial->read();
            if (byte == START_FLAG) {
                isWaiting = false;  // Signify that we are actively reading a message
                buffer[START_BYTE] = byte;
                timestamp = millis();
            }
            bytes++;
        } else {
            buffer[bufferLen + bytes] = serial->read();
            bytes++;
            if (!msgReady && bufferLen + bytes >= buffer[SIZE_BYTE] + HEADER_LENGTH) {
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

bool CommBuffer::hasMessage() {
    return msgReady;
}

Message* CommBuffer::getMessage() {
    if (!msgReady) return nullptr;
    msgReady = false;

    Message* msg = deserialize();
    cleanBuffer();

    return msg;
}

CommBuffer::~CommBuffer() {
    serial->end();
}
