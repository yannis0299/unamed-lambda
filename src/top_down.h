#ifndef TOP_DOWN_H
#define TOP_DOWN_H

#include "arena.h"
#include "prelude.h"
#include "token.h"
#include "translation_unit.h"
#include "vec.h"

typedef enum {
  STMT_PRAGMA,     // e.g {: infix(left, 5) :}
  STMT_MATCH_CASE, // e.g Node x l r => e
  STMT_DO_LET,     // e.g let x = e
  STMT_DO_BIND,    // e.g x <- read_line
  STMT_DO_EXPR,    // e.g ret -3
  STMT_BIND_TYPE,  // e.g foo : String
  STMT_BIND_EXPR,  // e.g foo = "bar"
} stmt_kind_t;

typedef struct {
  stmt_kind_t kind;
  slice_token_t lhs;
  slice_token_t rhs;
} stmt_t;

define_vec(stmt_t, stmt);

typedef enum {
  BLOCK_TOPLEVEL,
  BLOCK_MATCH,
  BLOCK_DO,
  BLOCK_WHERE,
} block_kind_t;

typedef struct {
  block_kind_t kind;
  usize column;
  vec_stmt_t stmts;
} block_t;

define_vec(block_t, block);

typedef struct {
  arena_t *arena;
  TU_t *tu;
  slice_token_t tokens;
  vec_block_t stack;
} top_down_t;

#endif // TOP_DOWN_H
