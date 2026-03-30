#include "arena.h"

#include "prelude.h"

#include <stdlib.h>
#include <string.h>

PtrResult arena_create(usize capacity) {
  Arena *arena;
  u8 *pool;

  arena = (Arena *)malloc(sizeof(Arena));
  pool = (u8 *)malloc(capacity);
  if (arena == NULL || pool == NULL) {
    return eyre_bail("ArenaError: Could not allocate byte pool "
                     "or the arena owner itself");
  }

  arena->capacity = capacity;
  arena->size = 0;
  arena->start = pool;
  arena->end = arena->start;

  return eyre_ok(arena);
}

void arena_destroy(Arena *arena) {
  if (arena) {
    if (arena->start)
      free(arena->start);
    free(arena);
  }
}

PtrResult arena_allocate(Arena *arena, usize size) {
  void *ptr;

  if (arena->size + size > arena->capacity) {
    return eyre_bail("ArenaError: Requested allocation amount "
                     "exeeds pool capacity (%zu)\n",
                     arena->capacity);
  }

  ptr = arena->end;
  arena->size += size;
  arena->end += size;

  return eyre_ok(ptr);
}

PtrResult arena_reallocate(Arena *arena, void *old_ptr, usize old_size,
                           usize new_size) {
  PtrResult res;
  void *new_ptr;

  res = arena_allocate(arena, new_size);
  eyre_guard(res);
  new_ptr = res.ok;
  memcpy(new_ptr, old_ptr, old_size);

  return eyre_ok(new_ptr);
}

void arena_deallocate(Arena *arena) {
  arena->size = 0;
  arena->end = arena->start;
}
