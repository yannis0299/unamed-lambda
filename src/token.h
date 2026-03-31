#ifndef TOKEN_H
#define TOKEN_H

#include "arena.h"
#include "prelude.h"
#include "vec.h"

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
} tokenkind_t;

typedef struct {
  tokenkind_t kind;
  usize line, column;
  usize start, span;
} token_t;

define_vec(token_t, token)

#endif // TOKEN_H
