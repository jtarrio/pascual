#include "file.h"

#include <errno.h>
#include <stdio.h>

#include "error.h"
#include "string.h"

static IoError error_code;
static PFile* error_file;

static inline void set_ioresult(PFile* file, IoError error) {
  error_code = error;
  error_file = file;
}

static inline void check_ioresult() {
  if (error_code == 0) return;
  io_error(error_code, error_file == NULL ? "unknown file"
                                          : pchar_of_str(&error_file->name));
}

int IORESULT() {
  int ret = error_code;
  set_ioresult(NULL, 0);
  return ret;
}

static inline int is_open(PFile* file) {
  if (file->file != NULL) return 1;
  set_ioresult(file, ieFileNotOpen);
  return 0;
}

void ASSIGN(PFile* file, PString name) {
  check_ioresult();
  file->name = name;
}

void CLOSE(PFile* file) {
  check_ioresult();
  if (!is_open(file)) return;
  fclose(file->file);
  file->file = NULL;
}

int EOF(PFile* file) {
  check_ioresult();
  if (!is_open(file)) return 1;
  clearerr(file->file);
  int ch = fgetc(file->file);
  if (feof(file->file)) return 1;
  ungetc(ch, file->file);
  return 0;
}

static inline void open_file(PFile* file, const char* mode) {
  check_ioresult();
  file->file = fopen(pchar_of_str(&file->name), mode);
  if (file->file == NULL) {
    set_ioresult(file, errno == ENOENT   ? ieFileNotFound
                       : errno == EACCES ? ieAccessDenied
                                         : ieUnknown);
  }
}

void RESET(PFile* file) { open_file(file, "r"); }

void REWRITE(PFile* file) { open_file(file, "w"); }

void READLN(PFile* file) {
  check_ioresult();
  if (!is_open(file)) return;
  clearerr(file->file);
  int chr;
  do {
    chr = fgetc(file->file);
  } while ((chr != '\n') && (chr != -1));
  if (ferror(file->file)) set_ioresult(file, ieReadError);
}

void READ_s(PFile* file, PString* str) {
  check_ioresult();
  if (!is_open(file)) return;
  clearerr(file->file);
  int chr = 0;
  int len = 0;
  while ((len < 255) && (chr != '\n')) {
    chr = fgetc(file->file);
    if (chr == -1)
      break;
    else if (chr == '\n')
      ungetc(chr, file->file);
    else
      str->value[len++] = chr;
  }
  str->len = len;
  if (ferror(file->file)) set_ioresult(file, ieReadError);
}

void WRITELN(PFile* file) {
  check_ioresult();
  if (!is_open(file)) return;
  clearerr(file->file);
  fputc('\n', file->file);
  if (ferror(file->file)) set_ioresult(file, ieWriteError);
}

void WRITE_b(PFile* file, int val) {
  check_ioresult();
  if (!is_open(file)) return;
  clearerr(file->file);
  fputs(val ? "TRUE" : "FALSE", file->file);
  if (ferror(file->file)) set_ioresult(file, ieWriteError);
}

void WRITE_i(PFile* file, int num) {
  check_ioresult();
  if (!is_open(file)) return;
  clearerr(file->file);
  fprintf(file->file, "%d", num);
  if (ferror(file->file)) set_ioresult(file, ieWriteError);
}

void WRITE_r(PFile* file, double num) {
  check_ioresult();
  if (!is_open(file)) return;
  clearerr(file->file);
  fprintf(file->file, "%g", num);
  if (ferror(file->file)) set_ioresult(file, ieWriteError);
}

void WRITE_c(PFile* file, char chr) {
  check_ioresult();
  if (!is_open(file)) return;
  clearerr(file->file);
  fputc(chr, file->file);
  if (ferror(file->file)) set_ioresult(file, ieWriteError);
}

void WRITE_s(PFile* file, PString str) {
  check_ioresult();
  if (!is_open(file)) return;
  clearerr(file->file);
  for (int pos = 0; pos < str.len; ++pos) fputc(str.value[pos], file->file);
  if (ferror(file->file)) set_ioresult(file, ieWriteError);
}

void WRITE_e(PFile* file, int value, const char** names) {
  check_ioresult();
  clearerr(file->file);
  if (!is_open(file)) return;
  fputs(names[value], file->file);
  if (ferror(file->file)) set_ioresult(file, ieWriteError);
}

PFile INPUT = {};
PFile OUTPUT = {};
PFile STDERR = {};

void InitFile() {
  error_code = 0;
  error_file = NULL;
  INPUT.file = stdin;
  INPUT.name = str_make(7, "(stdin)");
  OUTPUT.file = stdout;
  OUTPUT.name = str_make(8, "(stdout)");
  STDERR.file = stderr;
  STDERR.name = str_make(8, "(stderr)");
}