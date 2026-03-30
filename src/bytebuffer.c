#include "bytebuffer.h"

#include "arena.h"
#include "prelude.h"

#include <stdio.h>
#include <string.h>

PtrResult bytebuffer_create(Arena *arena) {
  PtrResult res;
  ByteBuffer *bb;

  res = arena_allocate(arena, sizeof(ByteBuffer));
  eyre_guard(res);
  bb = (ByteBuffer *)res.ok;

  res = arena_allocate(arena, BYTEBUFFER_DEFAULT_CAPACITY);
  eyre_guard(res);
  bb->buffer = (u8 *)res.ok;

  bb->capacity = BYTEBUFFER_DEFAULT_CAPACITY;
  bb->len = 0;

  return eyre_ok(bb);
}

PtrResult bytebuffer_read_contents(ByteBuffer *bb, const char *filename) {
  PtrResult res;
  FILE *fp;
  char chunk[1024];
  usize amount;

  fp = fopen(filename, "r");
  if (fp == NULL)
    return eyre_bail("ByteBuffer: Could not open file: %s", filename);

  while ((amount = fread(chunk, sizeof(char), 1024, fp))) {
    // Resize buffer if needed
    if (bb->len + amount - 1 >= bb->capacity) {
      res = arena_reallocate(bb->arena, bb->buffer, bb->capacity,
                             2 * bb->capacity);
      eyre_guard(res);
      bb->capacity *= 2;
      bb->buffer = res.ok;
    }
    // Append new chunk to buffer
    memcpy(bb->buffer + bb->len, chunk, (usize)amount);
    bb->len += (usize)amount;
  }

  if (ferror(fp)) { // Error reading file
    fclose(fp);
    return eyre_bail("ByteBuffer: Error while reading file %s", filename);
  }

  if (feof(fp)) { // Reached end of file
    fclose(fp);
    bb->buffer[bb->len] = '\0';
  }

  return eyre_ok(bb->buffer);
}
