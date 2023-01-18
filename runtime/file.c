#include "file.h"

#include <stdio.h>

void ASSIGN(PFile* file, PString name) {
  file->name = name;
  if (file->name.len < 255) file->name.value[file->name.len] = 0;
}

void CLOSE(PFile* file) { fclose(file->file); }

PBoolean EOF(PFile* file) {
  int ch = fgetc(file->file);
  if (ch == -1) return 1;
  ungetc(ch, file->file);
  return 0;
}

void RESET(PFile* file) { file->file = fopen(file->name.value, "r"); }

void REWRITE(PFile* file) { file->file = fopen(file->name.value, "w"); }

void readln(PFile file) {
  int chr;
  do {
    chr = fgetc(file.file);
  } while ((chr != '\n') && (chr != -1));
}

void read_s(PFile file, PString* str) {
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

void write_s(PFile file, PString str) {
  for (int pos = 0; pos < str.len; ++pos) fputc(str.value[pos], file.file);
}

void write_e(PFile file, int value, const char** names) {
  fputs(names[value], file.file);
}

PFile INPUT = {};
PFile OUTPUT = {};
PFile STDERR = {};

void InitFile() {
  INPUT.file = stdin;
  INPUT.name.len = 0;
  OUTPUT.file = stdout;
  OUTPUT.name.len = 0;
  STDERR.file = stderr;
  STDERR.name.len = 0;
}