#include "arena.h"

#include "eyre.h"
#include "prelude.h"

#include <stdlib.h>
#include <string.h>

arena_t arena_new(usize capacity) {
  u8 *pool;

  pool = (u8 *)malloc(capacity);
  if (pool == NULL) {
    eyre_bail("ArenaError: Could not allocate byte pool");
  }

  return (arena_t){.capacity = capacity, .size = 0, .start = pool, .end = pool};
}

void arena_drop(arena_t *self) {
  if (self && self->start)
    free(self->start);
}

void *arena_allocate(arena_t *self, usize size) {
  void *ptr;

  if (self->size + size > self->capacity) {
    eyre_bail("ArenaError: Requested allocation amount "
              "exeeds pool capacity (%zu)\n",
              self->capacity);
  }

  ptr = self->end;
  self->size += size;
  self->end += size;

  return ptr;
}

void *arena_reallocate(arena_t *self, void *old_ptr, usize old_size,
                       usize new_size) {
  void *new_ptr;

  new_ptr = arena_allocate(self, new_size);
  memcpy(new_ptr, old_ptr, old_size);

  return new_ptr;
}

void arena_deallocate(arena_t *self) {
  self->size = 0;
  self->end = self->start;
}
