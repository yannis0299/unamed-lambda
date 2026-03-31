#include "lexer.h"
#include "arena.h"
#include "token.h"

Lexer *lexer_create_from_tu(Arena *arena, TU *tu) {
  Lexer *lexer;

  lexer = (Lexer *)arena_allocate(arena, sizeof(Lexer));
  lexer->arena = arena;
  lexer->tu = tu;
  lexer->line = 1;
  lexer->column = 1;
  lexer->pos = 0;
  lexer->tokens = tokens_create(arena, 1);

  return lexer;
}

Token *lexer_emit_token(Lexer *lexer, TokenKind kind, usize span) {
  Token token = {.kind = kind,
                 .column = lexer->column,
                 .line = lexer->line,
                 .start = lexer->pos,
                 .span = span};
  tokens_push(lexer->tokens, token);
  return &lexer->tokens->ptr[lexer->tokens->len];
}

char lexer_advance_to_next_char(Lexer *lexer) {
  // Advance to next non-space character
  loop {
    if (lexer->pos >= lexer->tu->contents->len) {
      return '\0';
    } else {
      char c = (char)lexer->tu->contents->ptr[lexer->pos++];
      if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
        return c;
      } else if (c == ' ') {
        lexer->column++;
        continue;
      } else {
        lexer->column++;
        return c;
      }
    }
  }
}

Token *lexer_next(Lexer *lexer) {
  char c = lexer_advance_to_next_char(lexer);
  usize saved_line = lexer->line;
  usize saved_column = lexer->column;
  usize saved_pos = lexer->pos;
  switch (c) {
  case '\n':
    return lexer_emit_token(lexer, TOKENKIND_EOL, 1);
  case '\0':
    return lexer_emit_token(lexer, TOKENKIND_EOF, 1);
  case '\\':
    return lexer_emit_token(lexer, TOKENKIND_BACKSLASH, 1);
  case '(':
    return lexer_emit_token(lexer, TOKENKIND_LEFT_PAREN, 1);
  case ')':
    return lexer_emit_token(lexer, TOKENKIND_RIGHT_PAREN, 1);
  case ',':
    return lexer_emit_token(lexer, TOKENKIND_COMMA, 1);
  default:
    return NULL; // TODO
  }
}
