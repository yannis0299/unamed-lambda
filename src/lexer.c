#include "lexer.h"
#include "arena.h"

Lexer *lexer_create_from_tu(Arena *arena, TU *tu) {
  Lexer *lexer;

  lexer = (Lexer *)arena_allocate(arena, sizeof(Lexer));
  lexer->arena = arena;
  lexer->tu = tu;
  lexer->line = 1;
  lexer->column = 1;
  lexer->pos = 0;
  lexer->count = 0;
  lexer->capacity = 1;
  lexer->tokens = (Token *)arena_allocate(arena, sizeof(Token));

  return lexer;
}

Token *lexer_next(Lexer *lexer) {
  return NULL; // TODO
}
