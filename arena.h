#ifndef ARENA_H
#define ARENA_H

#include "prelude.h"

typedef struct {
  u8 *start, *end;
  usize capacity, size;
} Arena;

Arena *arena_create(usize capacity);

void arena_destroy(Arena *arena);

void *arena_allocate(Arena *arena, usize size);

void *arena_reallocate(Arena *arena, void *ptr, usize old_size, usize new_size);

void arena_deallocate(Arena *arena);

#define with_arena(var, cap)                                                   \
  for (var = arena_create(cap); var != NULL; arena_destroy(var), var = NULL)

#endif // ARENA_H
