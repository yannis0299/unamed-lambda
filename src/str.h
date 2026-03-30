#ifndef STR_H
#define STR_H

#include "arena.h"
#include "prelude.h"

typedef struct {
  Arena *arena;
  u8 *ptr;
  usize len, capacity;
} Str;

Str *str_create(Arena *arena, usize capacity);

Str *str_create_from(Arena *arena, const char *src);

void str_append_cstr(Str *str, const u8 *part, usize len);

#endif // STR_H
