#include "protocol.h"

void serialize_request(Buffer *b, Request *req) {
    write_uint32(b, req->burst);
    write_uint8(b, req->priority);
}

void deserialize_request(Buffer *b, Request *req) {
    req->burst = read_uint32(b);
    req->priority = read_uint8(b);
}

void serialize_response(Buffer *b, Response *res) {
    write_uint32(b, res->pid);
}

void deserialize_response(Buffer *b, Response *res) {
    res->pid = read_uint32(b);
}