#include "runtime.h"

#include <stdio.h>
#include <string.h>

STRING str_of(char chr) {
  STRING ret;
  ret.len = 1;
  ret.value[0] = chr;
  return ret;
}

STRING str_make(int len, const char* str) {
  STRING ret;
  ret.len = len;
  memcpy(ret.value, str, len);
  return ret;
}

STRING str_concat_CHAR_CHAR(char a, char b) {
  STRING ret;
  ret.len = 2;
  ret.value[0] = a;
  ret.value[1] = b;
  return ret;
}

STRING str_concat_CHAR_STRING(char a, STRING b) {
  memmove(b.value + 1, b.value, 254);
  b.value[0] = a;
  if (b.len < 255) ++b.len;
  return b;
}

STRING str_concat_STRING_CHAR(STRING a, char b) {
  if (a.len < 255) {
    a.value[a.len] = b;
    ++a.len;
  }
  return a;
}

STRING str_concat_STRING_STRING(STRING a, STRING b) {
  int cp = b.len;
  if (cp > (255 - a.len)) cp = 255 - a.len;
  memcpy(a.value + a.len, b.value, cp);
  a.len += cp;
  return a;
}

int str_compare_CHAR_CHAR(char a, char b) {
  return (unsigned char)(a) - (unsigned char)(b);
}

int str_compare_CHAR_STRING(char a, STRING b) {
  int c = str_compare_CHAR_CHAR(a, b.value[0]);
  if (c != 0) return c;
  return 1 - b.len;
}

int str_compare_STRING_CHAR(STRING a, char b) {
  if (a.len == 0) return -1;
  if (a.len == 1) return str_compare_CHAR_CHAR(a.value[0], b);
  return 1;
}

int str_compare_STRING_STRING(STRING a, STRING b) {
  for (int i = 0; (i < a.len) && (i < b.len); ++i) {
    int c = str_compare_CHAR_CHAR(a.value[i], b.value[i]);
    if (c != 0) return c;
  }
  return a.len - b.len;
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

void write_BOOLEAN(PFile file, PBoolean val) {
    fputs(val ? "TRUE" : "FALSE", file.file);
}

void write_INTEGER(PFile file, int num) {
  fprintf(file.file, "%d", num);
}

void write_CHAR(PFile file, char chr) {
  fputc(chr, file.file);
}

void write_STRING(PFile file, STRING str) {
  for (int pos = 0; pos < str.len; ++pos) fputc(str.value[pos], file.file);
}