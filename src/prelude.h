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
typedef int64_t i64;
typedef uint64_t u64;

/* Helpful macros */
#define loop for (;;)

#define whereami()                                                             \
  do {                                                                         \
    printf("whereami(%s:%d)\n", __FILE__, __LINE__);                           \
    fflush(stdout);                                                            \
  } while (0);

#endif // PRELUDE_H
