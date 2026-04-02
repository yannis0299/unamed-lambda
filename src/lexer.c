#include "lexer.h"

#include "arena.h"
#include "eyre.h"
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

usize lexer_integer(lexer_t *self) {
  lexer_state_t saved_state = self->state;
  usize span = 0;
  char c = lexer_peek_char(self);
  if (c == '-') {
    lexer_next_char(self);
    c = lexer_peek_char(self);
    if (isdigit(c)) {
      span = 2;
      lexer_next_char(self);
      while ((c = lexer_peek_char(self)) && isdigit(c)) {
        lexer_next_char(self);
        span++;
      }
      return span;
    } else {
      self->state = saved_state;
      return 0;
    }
  } else if (isdigit(c)) {
    span = 1;
    lexer_next_char(self);
    while ((c = lexer_peek_char(self)) && isdigit(c)) {
      lexer_next_char(self);
      span++;
    }
    return span;
  } else {
    self->state = saved_state;
    return 0;
  }
}

usize lexer_character(lexer_t *self) {
  lexer_state_t saved_state = self->state;
  usize span = 0;
  char c = lexer_peek_char(self);
  if (c == '\'') {
    lexer_next_char(self);
    span++;
    // Escape mechanism
    if ((c = lexer_peek_char(self)) && c == '\\') {
      lexer_next_char(self);
      span++;
    }
    // Consume inner character
    lexer_next_char(self);
    span++;
    // Check rightful character terminaison
    if ((c = lexer_peek_char(self)) && c == '\'') {
      lexer_next_char(self);
      span++;
      return span;
    } else {
      self->state = saved_state;
      return 0;
    }
  } else {
    self->state = saved_state;
    return 0;
  }
}

usize lexer_string(lexer_t *self) {
  lexer_state_t saved_state = self->state;
  usize span = 0;
  char c = lexer_peek_char(self);
  if (c == '"') {
    lexer_next_char(self);
    // Escape mechanism
    while ((c = lexer_peek_char(self)) && c != '"') {
      if ((c = lexer_peek_char(self)) && c == '\\') { // Eat escaped character
        lexer_next_char(self);
        span++;
      }
      // Consume inner character
      lexer_next_char(self);
      span++;
    }
    c = lexer_next_char(self); // consume terminaison character
    if (c == '"')
      return span;
    else {
      self->state = saved_state;
      return 0;
    }
  } else {
    self->state = saved_state;
    return 0;
  }
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
      if ((strlen(RESERVED_NAMES[idx]) == span) &&
          strncmp(repr, RESERVED_NAMES[idx], span) == 0) {
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

static const char *RESERVED_OPERATORS[] = {"\\", "=>", "=", ":", "<-",
                                           "..", "@",  "|", "#"};

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
      if ((strlen(RESERVED_OPERATORS[idx]) == span) &&
          strncmp(repr, RESERVED_OPERATORS[idx], span) == 0) {
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

usize lexer_comment(lexer_t *self) {
  lexer_state_t saved_state = self->state;
  usize span = 0;
  char c;
  if ((c = lexer_peek_char(self)) && c == '{') {
    lexer_next_char(self);
    span++;
    if ((c = lexer_peek_char(self)) && c == ':') {
      lexer_next_char(self);
      span++;
      loop {
        while ((c = lexer_peek_char(self)) && c != ':') {
          lexer_next_char(self);
          span++;
        }
        if (c == ':') {
          lexer_next_char(self);
          span++;
          if ((c = lexer_peek_char(self)) && c == '}') {
            lexer_next_char(self);
            span++;
            return span;
          } else {
            eyre_bail("lexer: Incorrect comment syntax");
            return 0;
          }
        } else {
          eyre_bail("lexer: Incorrect comment syntax");
          return 0;
        }
      }
    } else {
      self->state = saved_state;
      return 0;
    }
  } else {
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
  // Match comment for pragma purposes
  LEXER_MATCH(lexer_comment(self), TOKENKIND_COMMENT);
  // Match integer literal
  LEXER_MATCH(lexer_integer(self), TOKENKIND_INTEGER);
  // Match character literal
  LEXER_MATCH(lexer_character(self), TOKENKIND_CHARACTER);
  // Match string literal
  LEXER_MATCH(lexer_string(self), TOKENKIND_STRING);
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
  // Match left bracket character
  LEXER_MATCH(lexer_single(self, '['), TOKENKIND_LEFT_BRACKET);
  // Match right bracket character
  LEXER_MATCH(lexer_single(self, ']'), TOKENKIND_RIGHT_BRACKET);
  // Match comma character
  LEXER_MATCH(lexer_single(self, ','), TOKENKIND_COMMA);
  // Match right fat-arrow keyword
  LEXER_MATCH(lexer_multiple(self, "=>"), TOKENKIND_RIGHT_FATARROW);
  // Match equal character
  LEXER_MATCH(lexer_single(self, '='), TOKENKIND_EQUAL);
  // Match colon character
  LEXER_MATCH(lexer_single(self, ':'), TOKENKIND_COLON);
  // Match left arrow keyword
  LEXER_MATCH(lexer_multiple(self, "<-"), TOKENKIND_LEFT_ARROW);
  // Match .. keyword
  LEXER_MATCH(lexer_multiple(self, ".."), TOKENKIND_DOTDOT);
  // Match at character
  LEXER_MATCH(lexer_single(self, '@'), TOKENKIND_AT);
  // Match vertical bar character
  LEXER_MATCH(lexer_single(self, '|'), TOKENKIND_VERTICAL);
  // Match hashtag character
  LEXER_MATCH(lexer_single(self, '#'), TOKENKIND_HASHTAG);
  // Match match keyword token
  LEXER_MATCH(lexer_multiple(self, "match"), TOKENKIND_MATCH);
  // Match with keyword token
  LEXER_MATCH(lexer_multiple(self, "with"), TOKENKIND_WITH);
  // Match if keyword token
  LEXER_MATCH(lexer_multiple(self, "if"), TOKENKIND_IF);
  // Match then keyword token
  LEXER_MATCH(lexer_multiple(self, "then"), TOKENKIND_THEN);
  // Match else keyword token
  LEXER_MATCH(lexer_multiple(self, "else"), TOKENKIND_ELSE);
  // Match where keyword token
  LEXER_MATCH(lexer_multiple(self, "where"), TOKENKIND_WHERE);
  // Match let keyword token
  LEXER_MATCH(lexer_multiple(self, "let"), TOKENKIND_LET);
  // Match do keyword token
  LEXER_MATCH(lexer_multiple(self, "do"), TOKENKIND_DO);
  // If nothing is matched
  eyre_bail("lexer: Could not match any token at %zu:%zu", state.line,
            state.column);
  return NULL;
}

void lexer_exhaust(lexer_t *self) {
  token_t *token = NULL;
  do {
    token = lexer_next(self);
  } while (token != NULL && token->kind != TOKENKIND_EOF);
}
