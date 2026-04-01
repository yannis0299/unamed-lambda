#ifndef TOKEN_H
#define TOKEN_H

#include "arena.h"
#include "prelude.h"
#include "str.h"
#include "vec.h"

typedef enum {
  TOKENKIND_INTEGER,
  TOKENKIND_IDENTIFIER,
  TOKENKIND_OPERATOR,
  TOKENKIND_LEFT_PAREN,
  TOKENKIND_RIGHT_PAREN,
  TOKENKIND_LEFT_BRACKET,
  TOKENKIND_RIGHT_BRACKET,
  TOKENKIND_BACKSLASH,
  TOKENKIND_COMMA,
  TOKENKIND_EQUAL,
  TOKENKIND_COLON,
  TOKENKIND_RIGHT_FATARROW,
  TOKENKIND_LEFT_ARROW,
  TOKENKIND_DOTDOT,
  TOKENKIND_AT,
  TOKENKIND_VERTICAL,
  TOKENKIND_MATCH,
  TOKENKIND_WITH,
  TOKENKIND_IF,
  TOKENKIND_THEN,
  TOKENKIND_ELSE,
  TOKENKIND_WHERE,
  TOKENKIND_LET,
  TOKENKIND_DO,
  TOKENKIND_EOL,
  TOKENKIND_EOF,
  TOKENKIND_MAX_KIND,
} tokenkind_t;

static const char *TOKENKIND_SHOW_TABLE[TOKENKIND_MAX_KIND] = {
    "TOKENKIND_INTEGER",
    "TOKENKIND_IDENTIFIER",
    "TOKENKIND_OPERATOR",
    "TOKENKIND_LEFT_PAREN",
    "TOKENKIND_RIGHT_PAREN",
    "TOKENKIND_LEFT_BRACKET",
    "TOKENKIND_RIGHT_BRACKET",
    "TOKENKIND_BACKSLASH",
    "TOKENKIND_COMMA",
    "TOKENKIND_EQUAL",
    "TOKENKIND_COLON",
    "TOKENKIND_RIGHT_FATARROW",
    "TOKENKIND_LEFT_ARROW",
    "TOKENKIND_DOTDOT",
    "TOKENKIND_AT",
    "TOKENKIND_VERTICAL",
    "TOKENKIND_MATCH",
    "TOKENKIND_WITH",
    "TOKENKIND_IF",
    "TOKENKIND_THEN",
    "TOKENKIND_ELSE",
    "TOKENKIND_WHERE",
    "TOKENKIND_LET",
    "TOKENKIND_DO",
    "TOKENKIND_EOL",
    "TOKENKIND_EOF",
};

typedef struct {
  tokenkind_t kind;
  usize line, column;
  usize start, span;
  str_t repr;
} token_t;

define_vec(token_t, token);

static inline void token_show(token_t *self) {
  printf("Token { .kind = %s, .line = %zu, .column = %zu, .start = %zu, "
         ".span = %zu, .repr = \"%s\" }\n",
         TOKENKIND_SHOW_TABLE[self->kind], self->line, self->column,
         self->start, self->span,
         (self->kind != TOKENKIND_EOL) ? ((char *)self->repr.raw) : "\\n");
}

#endif // TOKEN_H
