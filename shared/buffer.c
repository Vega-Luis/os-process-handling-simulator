#include "buffer.h"
#include <string.h>
#include <arpa/inet.h>

void buffer_init(Buffer *b, uint8_t *data, size_t size) {
    b->data = data;
    b->size = size;
    b->offset = 0;
}

void write_uint32(Buffer *b, uint32_t value) {
    uint32_t net = htonl(value);
    memcpy(b->data + b->offset, &net, sizeof(uint32_t));
    b->offset += sizeof(uint32_t);
}

void write_uint8(Buffer *b, uint8_t value) {
    memcpy(b->data + b->offset, &value, sizeof(uint8_t));
    b->offset += sizeof(uint8_t);
}

uint32_t read_uint32(Buffer *b) {
    uint32_t net;
    memcpy(&net, b->data + b->offset, sizeof(uint32_t));
    b->offset += sizeof(uint32_t);
    return ntohl(net);
}

uint8_t read_uint8(Buffer *b) {
    uint8_t value;
    memcpy(&value, b->data + b->offset, sizeof(uint8_t));
    b->offset += sizeof(uint8_t);
    return value;
}
