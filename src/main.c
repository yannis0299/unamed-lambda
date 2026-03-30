#include <stdio.h>

#include "arena.h"
#include "prelude.h"
#include "tu.h"

i32 main() {
  Arena *arena;
  TU *tu;

  arena = arena_create(20 * 1024 * 1024);
  tu = tu_create_from_file(arena, "foo.ln");
  printf("foo.ln contents = %s \n", tu->contents->ptr);

  arena_deallocate(arena);
  arena_destroy(arena);

  return 0;
}
