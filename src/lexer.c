#include "lexer.h"

#include "arena.h"
#include "token.h"

lexer_t lexer_new_from_tu(arena_t *arena, TU_t *tu) {
  return (lexer_t){
      .arena = arena,
      .tu = tu,
      .line = 1,
      .column = 1,
      .pos = 0,
      .tokens = vec_token_new(arena, 1),
  };
}

token_t *lexer_emit_token(lexer_t *self, tokenkind_t kind, usize span) {
  token_t token = {.kind = kind,
                   .column = self->column,
                   .line = self->line,
                   .start = self->pos,
                   .span = span};
  vec_token_push(&self->tokens, token);
  return self->tokens.raw + self->tokens.len;
}

char lexer_advance_to_next_char(lexer_t *self) {
  // Advance to next non-space character
  loop {
    if (self->pos >= self->tu->contents.len) {
      return '\0';
    } else {
      char c = (char)self->tu->contents.raw[self->pos++];
      if (c == '\n') {
        self->line++;
        self->column = 1;
        return c;
      } else if (c == ' ') {
        self->column++;
        continue;
      } else {
        self->column++;
        return c;
      }
    }
  }
}

token_t *lexer_next(lexer_t *self) {
  char c = lexer_advance_to_next_char(self);
  usize saved_line = self->line;
  usize saved_column = self->column;
  usize saved_pos = self->pos;
  switch (c) {
  case '\n':
    return lexer_emit_token(self, TOKENKIND_EOL, 1);
  case '\0':
    return lexer_emit_token(self, TOKENKIND_EOF, 1);
  case '\\':
    return lexer_emit_token(self, TOKENKIND_BACKSLASH, 1);
  case '(':
    return lexer_emit_token(self, TOKENKIND_LEFT_PAREN, 1);
  case ')':
    return lexer_emit_token(self, TOKENKIND_RIGHT_PAREN, 1);
  case ',':
    return lexer_emit_token(self, TOKENKIND_COMMA, 1);
  default:
    return NULL; // TODO
  }
}
