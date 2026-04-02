#ifndef VEC_H
#define VEC_H

#include "arena.h"
#include "prelude.h"

#define define_vec(T, name)                                                    \
  typedef struct {                                                             \
    arena_t *arena;                                                            \
    usize len, cap;                                                            \
    T *raw;                                                                    \
  } vec_##name##_t;                                                            \
                                                                               \
  typedef struct {                                                             \
    T *ptr;                                                                    \
    usize len;                                                                 \
  } slice_##name##_t;                                                          \
                                                                               \
  static inline vec_##name##_t vec_##name##_new(arena_t *arena, usize cap) {   \
    return (vec_##name##_t){                                                   \
        .arena = arena,                                                        \
        .len = 0,                                                              \
        .cap = cap,                                                            \
        .raw = cap ? (T *)arena_allocate(arena, sizeof(T) * cap) : NULL};      \
  }                                                                            \
                                                                               \
  static inline void _vec_##name##_grow_if_needed(vec_##name##_t *self) {      \
    if (self->len >= self->cap) {                                              \
      usize old_cap = self->cap;                                               \
      self->cap = self->cap ? 2 * self->cap : 1;                               \
      self->raw = (T *)arena_reallocate(                                       \
          self->arena, self->raw, sizeof(T) * old_cap, sizeof(T) * self->cap); \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline void vec_##name##_push(vec_##name##_t *self, T elem) {         \
    _vec_##name##_grow_if_needed(self);                                        \
    self->raw[self->len++] = elem;                                             \
  }                                                                            \
                                                                               \
  static inline void vec_##name##_push_unsafe(vec_##name##_t *self, T elem) {  \
    self->raw[self->len++] = elem;                                             \
  }                                                                            \
                                                                               \
  static inline slice_##name##_t vec_##name##_slice(vec_##name##_t *self,      \
                                                    usize start, usize len) {  \
    return (slice_##name##_t){.ptr = &self->raw[start], .len = len};           \
  }                                                                            \
                                                                               \
  static inline slice_##name##_t vec_##name##_as_slice(vec_##name##_t *self) { \
    return (slice_##name##_t){.ptr = self->raw, .len = self->len};             \
  }                                                                            \
                                                                               \
  static inline T *vec_##name##_last(vec_##name##_t *self) {                   \
    return (self->len) ? (self->raw + self->len - 1) : NULL;                   \
  }                                                                            \
                                                                               \
  static inline void printf_vec_##name(vec_##name##_t *self) {                 \
    printf("[\n");                                                             \
    for (usize idx = 0; idx < self->len - 1; idx++) {                          \
      printf("  ");                                                            \
      printf_##name(self->raw + idx);                                          \
      printf(",\n");                                                           \
    }                                                                          \
    if (self->len > 0) {                                                       \
      printf("  ");                                                            \
      printf_##name(self->raw + self->len - 1);                                \
      printf("\n");                                                            \
    }                                                                          \
    printf("]\n");                                                             \
  }

#endif // VEC_H
