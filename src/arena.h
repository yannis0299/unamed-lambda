#ifndef ARENA_H
#define ARENA_H

#include "prelude.h"

typedef struct {
  u8 *start, *end;
  usize capacity, size;
} Arena;

PtrResult arena_create(usize capacity);

void arena_destroy(Arena *arena);

PtrResult arena_allocate(Arena *arena, usize size);

PtrResult arena_reallocate(Arena *arena, void *ptr, usize old_size,
                           usize new_size);

void arena_deallocate(Arena *arena);

#endif // ARENA_H
