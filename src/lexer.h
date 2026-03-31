#ifndef LEXER_H
#define LEXER_H

#include "arena.h"
#include "prelude.h"
#include "token.h"
#include "translation_unit.h"

typedef struct {
  Arena *arena;
  TU *tu;
  usize line, column, pos;
  Tokens *tokens;
} Lexer;

Lexer *lexer_create_from_tu(Arena *arena, TU *tu);

Token *lexer_next(Lexer *tk);

#endif // LEXER_H
