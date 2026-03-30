#include <stdio.h>

#include "arena.h"
#include "lexer.h"
#include "prelude.h"
#include "translation_unit.h"

i32 main() {
  Arena *arena;
  TU *tu;
  Lexer *lexer;
  Token *token;

  arena = arena_create(20 * 1024 * 1024);
  tu = tu_create_from_file(arena, "foo.ln");
  lexer = lexer_create_from_tu(arena, tu);
  printf("--- File: %10s ----------\n", tu->filename->ptr);
  printf("%s", tu->contents->ptr);
  printf("--- Tokens: -------------------\n");
  while ((token = lexer_next(lexer)) != NULL)
    printf("toto\n");

  arena_deallocate(arena);
  arena_destroy(arena);

  return 0;
}
