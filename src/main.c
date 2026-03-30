#include <stdio.h>

#include "arena.h"
#include "prelude.h"

i32 main() {
  printf("Hello, world!\n");
  Arena *arena;
  with_arena(arena, 2 * 1024 * 1024) {
    u8 *bytes = arena_allocate(arena, 1024);
    printf("b.0 = %c \n", bytes[0]);
    arena_deallocate(arena);
  }
  PtrResult res = {.ok = NULL,
                   .err = eyre("This is my first eyre from foo! %d", 24)};
  void *foo;
  foo = eyre_unpack(res);
  return 0;
}
