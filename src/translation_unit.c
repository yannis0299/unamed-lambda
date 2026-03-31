#include "translation_unit.h"

#include "arena.h"
#include "eyre.h"
#include "prelude.h"
#include "str.h"

#include <stdio.h>
#include <string.h>

TU_t tu_new_from_file(arena_t *arena, const char *filename) {
  FILE *fp;
  char chunk[TU_DEFAULT_CAPACITY];
  usize amount;

  TU_t self = (TU_t){
      .arena = arena,
      .filename = str_from(arena, filename),
      .contents = str_new(arena, TU_DEFAULT_CAPACITY),
  };

  fp = fopen(filename, "r");
  if (fp == NULL)
    eyre_bail("TU: Could not open file: %s", filename);

  while ((amount = fread(chunk, sizeof(char), TU_DEFAULT_CAPACITY, fp))) {
    // Append new chunk to buffer
    str_push_cstr(&self.contents, (u8 *)chunk, amount);
  }

  if (!feof(fp) || ferror(fp)) { // Error reading file
    eyre_bail("TU: Error while reading file: %s [%d]", filename, ferror(fp));
  }
  fclose(fp);

  return self;
}
