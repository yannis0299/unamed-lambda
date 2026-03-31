#ifndef ARENA_H
#define ARENA_H

#include "prelude.h"

typedef struct {
  u8 *start, *end;
  usize capacity, size;
} arena_t;

arena_t arena_new(usize capacity);

void arena_drop(arena_t *self);

void *arena_allocate(arena_t *self, usize size);

void *arena_reallocate(arena_t *self, void *ptr, usize old_size,
                       usize new_size);

void arena_deallocate(arena_t *self);

#endif // ARENA_H
