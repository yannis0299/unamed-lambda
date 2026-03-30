#include "tu.h"

#include "arena.h"
#include "prelude.h"
#include "str.h"

#include <stdio.h>
#include <string.h>

TU *tu_create_from_file(Arena *arena, const char *filename) {
  TU *tu;
  FILE *fp;
  char chunk[TU_DEFAULT_CAPACITY];
  usize amount;

  tu = (TU *)arena_allocate(arena, sizeof(TU));
  tu->arena = arena;
  tu->filename = str_create_from(arena, filename);
  tu->contents = str_create(arena, TU_DEFAULT_CAPACITY);

  fp = fopen(filename, "r");
  if (fp == NULL)
    eyre_bail("TU: Could not open file: %s", filename);

  while ((amount = fread(chunk, sizeof(char), TU_DEFAULT_CAPACITY, fp))) {
    // Append new chunk to buffer
    str_append_cstr(tu->contents, (u8 *)chunk, amount);
  }

  if (feof(fp)) { // Error reading file
    fclose(fp);
    return tu;
  } else if (ferror(fp)) {
    eyre_bail("TU: Error while reading file: %s [%d]", filename, ferror(fp));
    fclose(fp);
    return NULL;
  } else {
    eyre_bail("TU: Undefined error while reading file: %s", filename);
    fclose(fp);
    return NULL;
  }
}
