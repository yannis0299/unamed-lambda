#ifndef TU_H
#define TU_H

#include "arena.h"
#include "prelude.h"
#include "str.h"

#define TU_DEFAULT_CAPACITY 1024

typedef struct {
  Arena *arena;
  Str *filename;
  Str *contents;
} TU;

TU *tu_create_from_file(Arena *arena, const char *filename);

#endif // TU_H
