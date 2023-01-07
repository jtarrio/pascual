#include "pascual.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { reOutOfBounds = 0 } RtError;
const char* RtErrorMsgs[] = {"Index out of bounds"};

void rt_error(RtError err) {
  fprintf(stderr, "Run-time error: %s\n", RtErrorMsgs[err]);
  exit(1);
}

void HALT(int code) { exit(code); }

void ClampStringBounds(STRING* src, int* pos, int* len) {
  if (*pos < 1) rt_error(reOutOfBounds);
  if (*pos > src->len) *pos = src->len + 1;
  if (*len < 1) *len = 0;
  int max_len = 1 + src->len - *pos;
  if (*len > max_len) *len = max_len;
}

int LENGTH(STRING s) { return s.len; }

STRING COPY(STRING src, int pos, int num) {
  ClampStringBounds(&src, &pos, &num);
  STRING ret;
  if ((pos <= src.len) && (num > 0)) {
    ret.len = num;
    memcpy(ret.value, src.value + pos - 1, num);
  }
  return ret;
}

void DELETE(STRING* str, int pos, int num) {
  ClampStringBounds(str, &pos, &num);
  if ((pos <= str->len) && (num > 0)) {
    memmove(str->value + pos - 1, str->value + pos + num - 1, 255 - num);
  }
  str->len = str->len - num;
}

char UPCASE(char src) {
  if ((src >= 'a') && (src <= 'z')) return src - ('a' - 'A');
  return src;
}

STRING _str_from_pchar(const char* str) {
  STRING ret;
  ret.len = stpncpy(ret.value, str, 255) - str;
  return ret;
}

STRING str_of(char chr) {
  STRING ret;
  ret.len = 1;
  ret.value[0] = chr;
  return ret;
}

STRING cat_cc(char a, char b) {
  STRING ret;
  ret.len = 2;
  ret.value[0] = a;
  ret.value[1] = b;
  return ret;
}

STRING cat_cs(char a, STRING b) {
  memmove(b.value + 1, b.value, 254);
  b.value[0] = a;
  if (b.len < 255) ++b.len;
  return b;
}

STRING cat_sc(STRING a, char b) {
  if (a.len < 255) {
    a.value[a.len] = b;
    ++a.len;
  }
  return a;
}

STRING cat_ss(STRING a, STRING b) {
  int cp = b.len;
  if (cp > (255 - a.len)) cp = 255 - a.len;
  memcpy(a.value + a.len, b.value, cp);
  a.len += cp;
  return a;
}

int cmp_cc(char a, char b) { return (unsigned char)(a) - (unsigned char)(b); }

int cmp_cs(char a, STRING b) {
  int c = cmp_cc(a, b.value[0]);
  if (c != 0) return c;
  return 1 - b.len;
}

int cmp_sc(STRING a, char b) {
  if (a.len == 0) return -1;
  if (a.len == 1) return cmp_cc(a.value[0], b);
  return 1;
}

int cmp_ss(STRING a, STRING b) {
  for (int i = 0; (i < a.len) && (i < b.len); ++i) {
    int c = cmp_cc(a.value[i], b.value[i]);
    if (c != 0) return c;
  }
  return a.len - b.len;
}

STRING to_str_b(PBoolean val) {
  if (val)
    return str_make(4, "TRUE");
  else
    return str_make(5, "FALSE");
}

STRING to_str_i(int num) {
  STRING ret;
  ret.len = snprintf(ret.value, 255, "%d", num);
  return ret;
}

STRING to_str_c(char chr) { return str_of(chr); }

STRING to_str_s(STRING str) { return str; }

STRING to_str_e(int value, const char** names) {
  return _str_from_pchar(names[value]);
}

int param_count;
const char** param_str;

int PARAMCOUNT() {
  return param_count;
}

STRING PARAMSTR(int i) {
  if ((i < 1) || (i > param_count)) return (STRING){.len = 0};
  return _str_from_pchar(param_str[i - 1]);
}

void ASSIGN(PFile* file, STRING name) {
  file->name = name;
  if (file->name.len < 255) file->name.value[file->name.len] = 0;
}

void CLOSE(PFile* file) { fclose(file->file); }

PBoolean EOF(PFile* file) {
  int ch = fgetc(file->file);
  if (ch == -1) return TRUE;
  ungetc(ch, file->file);
  return FALSE;
}

void RESET(PFile* file) { file->file = fopen(file->name.value, "r"); }

void readln(PFile file) {
  int chr;
  do {
    chr = fgetc(file.file);
  } while ((chr != '\n') && (chr != -1));
}

void read_s(PFile file, STRING* str) {
  int chr = 0;
  int len = 0;
  while ((len < 255) && (chr != '\n')) {
    chr = fgetc(file.file);
    if (chr == -1)
      break;
    else if (chr == '\n')
      ungetc(chr, file.file);
    else
      str->value[len++] = chr;
  }
  str->len = len;
}

void writeln(PFile file) { fputc('\n', file.file); }

void write_b(PFile file, PBoolean val) {
  fputs(val ? "TRUE" : "FALSE", file.file);
}

void write_i(PFile file, int num) { fprintf(file.file, "%d", num); }

void write_c(PFile file, char chr) { fputc(chr, file.file); }

void write_s(PFile file, STRING str) {
  for (int pos = 0; pos < str.len; ++pos) fputc(str.value[pos], file.file);
}

void write_e(PFile file, int value, const char** names) {
  fputs(names[value], file.file);
}

const PBoolean FALSE = 0;
const PBoolean TRUE = !FALSE;

PFile INPUT = {};
PFile OUTPUT = {};
PFile STDERR = {};

extern void pascual_main();

int main(int argc, const char** argv) {
  INPUT.file = stdin;
  OUTPUT.file = stdout;
  STDERR.file = stderr;
  param_count = argc - 1;
  param_str = argv + 1;
  pascual_main();
  return 0;
}