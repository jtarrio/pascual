#include "runtime.h"

#include <stdio.h>
#include <string.h>

STRING str_make(int len, const char* str) {
  STRING ret;
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

void readln(PFile file) {
  int chr;
  do {
    chr = fgetc(file.file);
    if (chr == '\n') ungetc(chr, file.file);
  } while (chr != '\n');
}

void read_STRING(PFile file, STRING* str) {
  int chr = 0;
  int len = 0;
  while ((len < 255) && (chr != '\n')) {
    chr = fgetc(file.file);
    if (chr == '\n') {
      ungetc(chr, file.file);
    } else {
      str->value[len++] = chr;
    }
  }
  str->len = len;
}

void writeln(PFile file) { fputc('\n', file.file); }

void write_INTEGER(PFile file, int num) {
  fprintf(file.file, "%d", num);
}

void write_CHAR(PFile file, char chr) {
  fputc(chr, file.file);
}

void write_STRING(PFile file, STRING str) {
  for (int pos = 0; pos < str.len; ++pos) fputc(str.value[pos], file.file);
}