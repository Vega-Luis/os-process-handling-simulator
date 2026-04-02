#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "buffer.h"
#include "request_message.h"
#include "response_message.h"

#define REQUEST_SIZE (sizeof(uint32_t) + sizeof(uint8_t))
#define RESPONSE_SIZE (sizeof(uint32_t))

void serialize_request(Buffer *b, Request *req);
void deserialize_request(Buffer *b, Request *req);

void serialize_response(Buffer *b, Response *res);
void deserialize_response(Buffer *b, Response *res);

#endif