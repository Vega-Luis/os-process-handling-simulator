#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t *data;
    size_t size;
    size_t offset;
} Buffer;

void buffer_init(Buffer *b, uint8_t *data, size_t size);

// write
void write_uint32(Buffer *b, uint32_t value);
void write_uint8(Buffer *b, uint8_t value);

// read
uint32_t read_uint32(Buffer *b);
uint8_t read_uint8(Buffer *b);

#endif