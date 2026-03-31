#ifndef STR_H
#define STR_H

#include "arena.h"
#include "prelude.h"

typedef struct {
  arena_t *arena;
  u8 *raw;
  usize len, cap;
} str_t;

str_t str_new(arena_t *arena, usize capacity);

str_t str_from(arena_t *arena, const char *src);

void str_push(str_t *self, char c);

void str_push_cstr(str_t *self, const u8 *part, usize len);

#endif // STR_H
