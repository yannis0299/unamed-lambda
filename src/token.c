#include "token.h"

#include "arena.h"
#include "prelude.h"

Tokens *tokens_create(Arena *arena, usize capacity) {
  Tokens *tokens;

  veclike_create(Tokens, Token, tokens);

  return tokens;
}

void tokens_push(Tokens *tokens, Token token) {
  veclike_push(Tokens, Token, tokens, token);
}
