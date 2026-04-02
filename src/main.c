#include "arena.h"
#include "lexer.h"
#include "prelude.h"
#include "token.h"
#include "translation_unit.h"

#include <stdio.h>

i32 main() {
  /* Init */
  arena_t arena = arena_new(20 * 1024 * 1024);

  /* Body */
  TU_t tu = tu_new_from_file(&arena, "foo.ln");
  printf("--- File: %10s ----------\n", tu.filename.raw);
  printf("%s", tu.contents.raw);

  printf("--- Tokens: -------------------\n");
  lexer_t lexer = lexer_new_from_tu(&arena, &tu);
  lexer_exhaust(&lexer);
  printf_vec_token(&lexer.tokens);

  /* Cleanup */
  arena_deallocate(&arena);
  arena_drop(&arena);

  return 0;
}
