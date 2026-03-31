#ifndef TOKEN_H
#define TOKEN_H

#include "arena.h"
#include "prelude.h"

typedef enum {
  TOKENKIND_IDENTIFIER,
  TOKENKIND_LEFT_PAREN,
  TOKENKIND_RIGHT_PAREN,
  TOKENKIND_COMMA,
  TOKENKIND_EQUAL,
  TOKENKIND_COLON,
  TOKENKIND_SEMICOLON,
  TOKENKIND_BACKSLASH,
  TOKENKIND_RIGHT_FATARROW,
  TOKENKIND_EOL,
  TOKENKIND_EOF,
} TokenKind;

typedef struct {
  TokenKind kind;
  usize line, column;
  usize start, span;
} Token;

typedef struct {
  veclike_definition(Token)
} Tokens;

Tokens *tokens_create(Arena *arena, usize capacity);

void tokens_push(Tokens *tokens, Token token);

#endif // TOKEN_H
