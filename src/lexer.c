#include "lexer.h"

#include "arena.h"
#include "prelude.h"
#include "str.h"
#include "token.h"

#include <ctype.h>
#include <string.h>

lexer_t lexer_new_from_tu(arena_t *arena, TU_t *tu) {
  return (lexer_t){
      .arena = arena,
      .tu = tu,
      .state =
          (lexer_state_t){
              .line = 1,
              .column = 1,
              .pos = 0,
          },
      .tokens = vec_token_new(arena, 1),
  };
}

char lexer_peek_char(lexer_t *self) {
  if (self->state.pos >= self->tu->contents.len)
    return '\0';
  else
    return self->tu->contents.raw[self->state.pos];
}

char lexer_next_char(lexer_t *self) {
  if (self->state.pos >= self->tu->contents.len)
    return '\0';
  else {
    char c = (char)self->tu->contents.raw[self->state.pos++];
    if ('\n' == c) {
      self->state.line++;
      self->state.column = 1;
    } else
      self->state.column++;
    return c;
  }
}

usize lexer_single(lexer_t *self, char e) {
  char c = lexer_peek_char(self);
  if (e == c) {
    lexer_next_char(self);
    return 1;
  } else
    return 0;
}

usize lexer_multiple(lexer_t *self, const char *e) {
  lexer_state_t saved_state = self->state;
  usize len = strlen(e);
  char c;
  for (usize idx = 0; idx < len; idx++) {
    if ((c = lexer_peek_char(self)) && e[idx] == c)
      lexer_next_char(self);
    else {
      self->state = saved_state;
      return 0;
    }
  }
  return len;
}

static const char *RESERVED_NAMES[] = {"match", "with", "if",    "then",
                                       "else",  "let",  "where", "do"};

usize lexer_identifier(lexer_t *self) {
  lexer_state_t saved_state = self->state;
  usize span = 0;
#define ident_start(c) (c == '_' || isalpha(c))
#define ident_letter(c) (c == '_' || isalnum(c))
  char c = lexer_peek_char(self);
  if (ident_start(c)) { // consume char
    lexer_next_char(self);
    span = 1;
    while ((c = lexer_peek_char(self)) && ident_letter(c)) {
      lexer_next_char(self);
      span++;
    }
    // Check if identifier is in reserved namespace
    char *repr = (char *)self->tu->contents.raw + saved_state.pos;
    usize len = sizeof(RESERVED_NAMES) / sizeof(RESERVED_NAMES[0]);
    for (usize idx = 0; idx < len; idx++) {
      if (strncmp(repr, RESERVED_NAMES[idx], span) == 0) {
        self->state = saved_state;
        return 0;
      }
    }
    return span;
  } else { // or restore state and exit
    self->state = saved_state;
    return 0;
  }
}

static const char *RESERVED_OPERATORS[] = {"\\", "=>", "=", ":",
                                           "<-", "..", "@", "|"};

usize lexer_operator(lexer_t *self) {
  lexer_state_t saved_state = self->state;
  usize span = 0;
// ':', '!', '#', '$', '%', '&', '*', '+', '.', '/', '<', '=', '>', '@', '\\',
// '^', '|', '-', '~'
#define op_letter(next)                                                        \
  (next == ':' || next == '!' || next == '#' || next == '$' || next == '%' ||  \
   next == '&' || next == '*' || next == '+' || next == '.' || next == '/' ||  \
   next == '<' || next == '=' || next == '>' || next == '@' || next == '\\' || \
   next == '^' || next == '|' || next == '-' || next == '~')
  char c = lexer_peek_char(self);
  if (op_letter(c)) { // consume char
    lexer_next_char(self);
    span = 1;
    while ((c = lexer_peek_char(self)) && op_letter(c)) {
      lexer_next_char(self);
      span++;
    }
    // Check if identifier is in reserved operator table
    char *repr = (char *)self->tu->contents.raw + saved_state.pos;
    usize len = sizeof(RESERVED_OPERATORS) / sizeof(RESERVED_OPERATORS[0]);
    for (usize idx = 0; idx < len; idx++) {
      if (strncmp(repr, RESERVED_OPERATORS[idx], span) == 0) {
        self->state = saved_state;
        return 0;
      }
    }
    return span;
  } else { // or restore state and exit
    self->state = saved_state;
    return 0;
  }
}

token_t *lexer_next(lexer_t *self) {
  // Eat whitespaces prefixing characters
  char c;
  while ((c = lexer_peek_char(self)) && c == ' ')
    lexer_next_char(self);

  usize span = 0;
  lexer_state_t state = self->state;
#define LEXER_MATCH(matcher, _kind)                                            \
  span = matcher;                                                              \
  if (span) {                                                                  \
    str_t repr = str_new(self->arena, span + 1);                               \
    str_push_cstr(&repr, (u8 *)self->tu->contents.raw + state.pos, span);      \
    token_t token = {.kind = _kind,                                            \
                     .column = state.column,                                   \
                     .line = state.line,                                       \
                     .start = state.pos,                                       \
                     .span = span,                                             \
                     .repr = repr};                                            \
    vec_token_push(&self->tokens, token);                                      \
    return &self->tokens.raw[self->tokens.len - 1];                            \
  }

  // Match identifier token
  LEXER_MATCH(lexer_identifier(self), TOKENKIND_IDENTIFIER);
  // Match operator token
  LEXER_MATCH(lexer_operator(self), TOKENKIND_OPERATOR);
  // Match end of line character
  LEXER_MATCH(lexer_single(self, '\n'), TOKENKIND_EOL);
  // Match end of file character
  LEXER_MATCH(lexer_single(self, '\0'), TOKENKIND_EOF);
  // Match backslash character
  LEXER_MATCH(lexer_single(self, '\\'), TOKENKIND_BACKSLASH);
  // Match left paren character
  LEXER_MATCH(lexer_single(self, '('), TOKENKIND_LEFT_PAREN);
  // Match right paren character
  LEXER_MATCH(lexer_single(self, ')'), TOKENKIND_RIGHT_PAREN);
  // Match comma character
  LEXER_MATCH(lexer_single(self, ','), TOKENKIND_COMMA);
  // Match right fat-arrow
  LEXER_MATCH(lexer_multiple(self, "=>"), TOKENKIND_RIGHT_FATARROW);
  // Match equal character
  LEXER_MATCH(lexer_single(self, '='), TOKENKIND_EQUAL);
  // Match colon character
  LEXER_MATCH(lexer_single(self, ':'), TOKENKIND_COLON);
  // If nothing is matched
  return NULL;
}
