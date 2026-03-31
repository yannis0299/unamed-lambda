#ifndef LEXER_H
#define LEXER_H

#include "arena.h"
#include "prelude.h"
#include "token.h"
#include "translation_unit.h"

typedef struct {
  arena_t *arena;
  TU_t *tu;
  usize line, column, pos;
  vec_token_t tokens;
} lexer_t;

lexer_t lexer_new_from_tu(arena_t *arena, TU_t *tu);

token_t *lexer_next(lexer_t *self);

#endif // LEXER_H
