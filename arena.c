#include "arena.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Arena *arena_create(usize capacity) {
  Arena *arena;
  u8 *pool;

  arena = (Arena *)malloc(sizeof(Arena));
  pool = (u8 *)malloc(capacity);
  if (arena == NULL || pool == NULL) {
    fprintf(
        stderr,
        "ArenaError: Could not allocate byte pool or the arena owner itself\n");
    exit(1);
  }

  arena->capacity = capacity;
  arena->size = 0;
  arena->start = pool;
  arena->end = arena->start;

  return arena;
}

void arena_destroy(Arena *arena) {
  free(arena->start);
  free(arena);
}

void *arena_allocate(Arena *arena, usize size) {
  if (arena->size + size > arena->capacity) {
    fprintf(
        stderr,
        "ArenaError: Requested allocation amount exeeds pool capacity (%zu)\n",
        arena->capacity);
    exit(1);
  }
  arena->size += size;
  return (arena->end += size);
}

void *arena_reallocate(Arena *arena, void *old_ptr, usize old_size,
                       usize new_size) {
  void *new_ptr = arena_allocate(arena, new_size);
  memcpy(new_ptr, old_ptr, old_size);
  return new_ptr;
}

void arena_deallocate(Arena *arena) {
  arena->size = 0;
  arena->end = arena->start;
}
