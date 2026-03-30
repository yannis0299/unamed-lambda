#ifndef PRELUDE_H
#define PRELUDE_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef size_t usize;
typedef unsigned char u8;
typedef char i8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int16_t i16;
typedef uint16_t u16;

/* Eyre manipulation */
#define EYRE_MAX_MESSAGE_LENGTH 2048

#define eyre(fmt, ...)                                                         \
  (eyre_eyre(__FILE__, __LINE__, fmt __VA_OPT__(, ) __VA_ARGS__))

typedef struct {
  const char *filename;
  const usize line;
  char *message;
} Eyre;

typedef struct {
  void *ok;
  Eyre err;
} PtrResult;

static inline Eyre eyre_eyre(const char *filename, const usize line,
                             const char *fmt, ...) {
  Eyre err = {
      .filename = filename,
      .line = line,
      .message = (char *)malloc(EYRE_MAX_MESSAGE_LENGTH),
  };
  va_list va_args;
  va_start(va_args, fmt);
  vsnprintf((char *)err.message, EYRE_MAX_MESSAGE_LENGTH, fmt, va_args);
  va_end(va_args);
  return err;
}

static inline void *eyre_unpack(PtrResult res) {
  if (res.ok == NULL) {
    fprintf(stderr, "Eyre: %s:%zu\n\t%s\n", res.err.filename, res.err.line,
            res.err.message);
    exit(1);
  } else {
    return res.ok;
  }
}

#endif // PRELUDE_H
