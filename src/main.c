#include <stdio.h>

#include "arena.h"
#include "bytebuffer.h"
#include "prelude.h"

i32 main() {
  Arena *arena;
  ByteBuffer *bb;
  u8 *contents;

  arena = eyre_unpack(arena_create(20 * 1024 * 1024));
  bb = eyre_unpack(bytebuffer_create(arena));
  contents = eyre_unpack(bytebuffer_read_contents(bb, "foo.ln"));
  printf("foo.ln contents = %s \n", contents);

  arena_deallocate(arena);
  arena_destroy(arena);

  return 0;
}
