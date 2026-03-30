#ifndef LEXER_H
#define LEXER_H

#include "arena.h"
#include "prelude.h"
#include "translation_unit.h"

typedef enum {
  TOKEN_IDENTIFIER,
  TOKEN_LEFT_PAREN,
  TOKEN_RIGHT_PAREN,
  TOKEN_EQUAL,
  TOKEN_BACKSLASH,
  TOKEN_RIGHT_FATARROW,
  TOKEN_EOL,
  TOKEN_EOF,
} TokenKind;

typedef struct {
  TokenKind kind;
  usize line, column;
  usize start, span;
} Token;

typedef struct {
  Arena *arena;
  TU *tu;
  usize line, column, pos;
  Token *tokens;
  usize count, capacity;
} Lexer;

Lexer *lexer_create_from_tu(Arena *arena, TU *tu);

Token *lexer_next(Lexer *tk);

#endif // LEXER_H
