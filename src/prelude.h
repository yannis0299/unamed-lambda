#ifndef PRELUDE_H
#define PRELUDE_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Generic type-redefinition */
typedef size_t usize;
typedef ptrdiff_t isize;
typedef unsigned char u8;
typedef char i8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int16_t i16;
typedef uint16_t u16;

/* Helpful macros */
#define loop for (;;)

#define whereami()                                                             \
  do {                                                                         \
    printf("I am here %s:%d \n", __FILE__, __LINE__);                          \
    fflush(stdout);                                                            \
  } while (0);

#define veclike_definition(T)                                                  \
  Arena *arena;                                                                \
  T *ptr;                                                                      \
  usize len, capacity;

#define veclike_create(TS, T, vec)                                             \
  do {                                                                         \
    vec = (TS *)arena_allocate(arena, sizeof(TS));                             \
    vec->arena = arena;                                                        \
    vec->capacity = capacity ? capacity : 1;                                   \
    vec->len = 0;                                                              \
    vec->ptr = (T *)arena_allocate(arena, sizeof(T) * vec->capacity);          \
  } while (0);

#define veclike_push(TS, T, vec, elem)                                         \
  do {                                                                         \
    if (vec->len >= vec->capacity) {                                           \
      vec->ptr = (T *)arena_reallocate(vec->arena, vec->ptr,                   \
                                       sizeof(T) * vec->capacity,              \
                                       sizeof(T) * 2 * vec->capacity);         \
      vec->capacity *= 2;                                                      \
    }                                                                          \
    vec->ptr[vec->len++] = elem;                                               \
  } while (0);

/* Eyre manipulation */
#define EYRE_MAX_MESSAGE_LENGTH 2048

#define eyre(fmt, ...)                                                         \
  (eyre_eyre(__FILE__, __LINE__, fmt __VA_OPT__(, ) __VA_ARGS__))

#define eyre_bail(fmt, ...)                                                    \
  do {                                                                         \
    Eyre err = eyre(fmt __VA_OPT__(, ) __VA_ARGS__);                           \
    eyre_panic(err);                                                           \
  } while (0);

typedef struct {
  char *filename;
  usize line;
  char *message;
} Eyre;

static inline Eyre eyre_eyre(const char *filename, const usize line,
                             const char *fmt, ...) {
  Eyre err = {
      .filename = (char *)filename,
      .line = line,
      .message = (char *)malloc(EYRE_MAX_MESSAGE_LENGTH),
  };
  va_list va_args;
  va_start(va_args, fmt);
  vsnprintf((char *)err.message, EYRE_MAX_MESSAGE_LENGTH, fmt, va_args);
  va_end(va_args);
  return err;
}

static inline void eyre_panic(Eyre err) {
  fprintf(stderr, "Eyre: %s:%zu\n\t%s\n", err.filename, err.line, err.message);
  exit(1);
}

#endif // PRELUDE_H
