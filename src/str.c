#include "str.h"

#include "arena.h"

#include <string.h>

str_t str_new(arena_t *arena, usize capacity) {
  usize cap = capacity ? capacity : 1;
  u8 *raw = (u8 *)arena_allocate(arena, cap);
  raw[0] = '\0';
  return (str_t){
      .arena = arena,
      .cap = cap,
      .len = 0,
      .raw = raw,
  };
}

str_t str_from(arena_t *arena, const char *src) {
  usize len = strlen(src);
  str_t self = str_new(arena, len + 1);
  strncpy((char *)self.raw, src, len);
  self.raw[self.len] = '\0';
  return self;
}

void _str_grow_if_needed(str_t *self) {
  if (self->len + 1 >= self->cap) {
    usize old_cap = self->cap;
    self->cap = self->cap ? 2 * self->cap : 1;
    self->raw =
        (u8 *)arena_reallocate(self->arena, self->raw, old_cap, self->cap);
  }
}

void str_push(str_t *self, char c) {
  _str_grow_if_needed(self);
  self->raw[self->len++] = (u8)c;
  self->raw[self->len] = '\0';
}

void str_push_cstr(str_t *self, const u8 *part, usize len) {
  while (self->len + len + 1 >= self->cap)
    _str_grow_if_needed(self);
  strncpy((char *)(self->raw + self->len), (char *)part, len);
  self->len += len;
  self->raw[self->len] = '\0';
}
