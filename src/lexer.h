#ifndef LEXER_H
#define LEXER_H

#include "arena.h"
#include "prelude.h"
#include "token.h"
#include "translation_unit.h"

typedef struct {
  usize line, column, pos;
} lexer_state_t;

typedef struct {
  arena_t *arena;
  TU_t *tu;
  vec_token_t tokens;
  lexer_state_t state;
} lexer_t;

lexer_t lexer_new_from_tu(arena_t *arena, TU_t *tu);

char lexer_peek_char(lexer_t *self);

char lexer_next_char(lexer_t *self);

token_t *lexer_next(lexer_t *self);

#endif // LEXER_H
