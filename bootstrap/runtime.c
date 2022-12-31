#include "runtime.h"

#include <stdio.h>
#include <string.h>

PString str_make(int len, const char* str) {
  PString ret;
  ret.len = len;
  memcpy(ret.value, str, len);
  return ret;
}

const PBoolean FALSE = 0;
const PBoolean TRUE = !FALSE;

void InitFiles() {
  INPUT.file = stdin;
  OUTPUT.file = stdout;
  STDERR.file = stderr;
}

PFile INPUT = {};
PFile OUTPUT = {};
PFile STDERR = {};

void writeln(PFile file) { fputc('\n', file.file); }

void write_STRING(PFile file, PString str) {
  for (int pos = 0; pos < str.len; ++pos) fputc(str.value[pos], file.file);
}