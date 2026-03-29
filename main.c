#include <stdio.h>

#include "arena.h"
#include "prelude.h"

#define with_arena(var, cap)                                                   \
  for (var = arena_create(cap); var != NULL; arena_destroy(var), var = NULL)

i32 main() {
  printf("Hello, world!\n");
  Arena *arena;
  with_arena(arena, 2 * 1024 * 1024) {
    u8 *bytes = arena_allocate(arena, 1024);
    printf("b.0 = %c \n", bytes[0]);
    arena_deallocate(arena);
  }
  return 0;
}
