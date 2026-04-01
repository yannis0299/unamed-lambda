#include <ctype.h>
#include <stdio.h>

#include "arena.h"
#include "lexer.h"
#include "prelude.h"
#include "token.h"
#include "translation_unit.h"

i32 main() {
  arena_t arena = arena_new(20 * 1024 * 1024);
  TU_t tu = tu_new_from_file(&arena, "foo.ln");
  lexer_t lexer = lexer_new_from_tu(&arena, &tu);

  printf("--- File: %10s ----------\n", tu.filename.raw);
  printf("%s", tu.contents.raw);

  printf("--- Tokens: -------------------\n");

  /* token_t *token = NULL; */
  /* for (usize acc = 0; acc < 148; acc++) { */
  /*   token = lexer_next(&lexer); */
  /*   if (token != NULL) */
  /*     token_show(token); */
  /*   else */
  /*     printf("Token { null } \n"); */
  /* } */

  token_t *token = NULL;
  do {
    token = lexer_next(&lexer);
    if (token)
      token_show(token);
  } while (token != NULL && token->kind != TOKENKIND_EOF);

  arena_deallocate(&arena);
  arena_drop(&arena);

  return 0;
}
