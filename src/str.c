#include "str.h"
#include "arena.h"

#include <string.h>

Str *str_create(Arena *arena, usize capacity) {
  Str *str;

  str = (Str *)arena_allocate(arena, sizeof(Str));
  str->arena = arena;
  str->capacity = capacity;
  str->len = 0;
  str->ptr = (u8 *)arena_allocate(arena, capacity);

  return str;
}

Str *str_create_from(Arena *arena, const char *src) {
  Str *str;
  usize len;

  len = strlen(src);
  str = str_create(arena, len + 1);
  strncpy((char *)str->ptr, src, len);
  str->len += len;
  str->ptr[str->len++] = '\0';

  return str;
}

void str_append_cstr(Str *str, const u8 *part, usize len) {
  if (str->len + len + 1 >= str->capacity) {
    str->ptr = (u8 *)arena_reallocate(str->arena, str->ptr, str->capacity,
                                      2 * str->capacity);
    str->capacity *= 2;
  }
  strncpy((char *)(str->ptr + str->len), (char *)part, len);
  str->len += len;
  str->ptr[str->len++] = '\0';
}
