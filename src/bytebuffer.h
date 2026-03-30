#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include "arena.h"
#include "prelude.h"

#define BYTEBUFFER_DEFAULT_CAPACITY 1024

typedef struct {
  Arena *arena;
  u8 *buffer;
  usize len, capacity;
} ByteBuffer;

PtrResult bytebuffer_create(Arena *arena);

PtrResult bytebuffer_read_contents(ByteBuffer *bb, const char *filename);

#endif // BYTEBUFFER_H
