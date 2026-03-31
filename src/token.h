#ifndef TOKEN_H
#define TOKEN_H

#include "arena.h"
#include "prelude.h"

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
  veclike_definition(Token)
} Tokens;

Tokens *tokens_create(Arena *arena, usize capacity);

void tokens_push(Tokens *tokens, Token token);

#endif // TOKEN_H
