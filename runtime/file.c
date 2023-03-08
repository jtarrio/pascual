#include "file.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "error.h"
#include "string.h"

static IoError error_code;
static const PFile* error_file;

static inline void set_ioresult(const PFile* file, IoError error) {
  error_code = error;
  error_file = file;
}

static IoError ioerror_from_errno(void) {
  switch (errno) {
    case EIO:
      return ieIoError;
    case ENOENT:
      return ieFileNotFound;
    case EACCES:
      return ieAccessDenied;
    case EBADF:
      return ieFileNotOpen;
    case ENOTDIR:
      return ieNotADirectory;
    default:
      return ieUnknown;
  }
}

static inline void check_ioresult() {
  if (error_code == 0) return;
  io_error(error_code, error_file == NULL ? "unknown file"
                                          : pchar_of_str(&error_file->name));
}

PInteger IORESULT() {
  int ret = error_code;
  set_ioresult(NULL, 0);
  return ret;
}

void CHDIR(const PString* dir) {
  check_ioresult();
  if (!chdir(pchar_of_str(dir))) set_ioresult(NULL, ioerror_from_errno());
}

void MKDIR(const PString* dir) {
  check_ioresult();
  if (!mkdir(pchar_of_str(dir), 0755)) set_ioresult(NULL, ioerror_from_errno());
}

void RMDIR(const PString* dir) {
  check_ioresult();
  if (!rmdir(pchar_of_str(dir))) set_ioresult(NULL, ioerror_from_errno());
}

void GETDIR(PInteger drive, PString* dir) {
  char buf[256];
  size_t buf_size = 256;
  check_ioresult();
  if (getcwd(buf, buf_size) == NULL)
    set_ioresult(NULL, ioerror_from_errno());
  else
    *dir = str_of_pchar(buf, 0);
}

static inline PBoolean is_open(const PFile* file) {
  if (file->file != NULL) return 1;
  set_ioresult(file, ieFileNotOpen);
  return 0;
}

void ASSIGN(PFile* file, const PString* name) {
  check_ioresult();
  file->name = *name;
}

void CLOSE(PFile* file) {
  check_ioresult();
  if (!is_open(file)) return;
  fclose(file->file);
  file->file = NULL;
}

PBoolean EOF(PFile* file) {
  check_ioresult();
  if (!is_open(file)) return 1;
  clearerr(file->file);
  int ch = fgetc(file->file);
  if (feof(file->file)) return 1;
  ungetc(ch, file->file);
  return 0;
}

PBoolean SEEKEOF(PFile* file) {
  check_ioresult();
  if (!is_open(file)) return 1;
  clearerr(file->file);
  int ch;
  do {
    ch = fgetc(file->file);
    if (feof(file->file)) return 1;
  } while (ch == ' ' || ch == '\t');
  ungetc(ch, file->file);
  return 0;
}

PBoolean EOLN(PFile* file) {
  check_ioresult();
  if (!is_open(file)) return 1;
  clearerr(file->file);
  int ch = fgetc(file->file);
  if (feof(file->file)) return 1;
  ungetc(ch, file->file);
  return ch == '\n';
}

PBoolean SEEKEOLN(PFile* file) {
  check_ioresult();
  if (!is_open(file)) return 1;
  clearerr(file->file);
  int ch;
  do {
    ch = fgetc(file->file);
    if (feof(file->file)) return 1;
  } while (ch == ' ' || ch == '\t');
  ungetc(ch, file->file);
  return ch == '\n';
}

PInteger FILEPOS(const PFile* file) {
  check_ioresult();
  if (!is_open(file)) return 0;
  PInteger pos = ftell(file->file);
  if (pos < 0) set_ioresult(file, ioerror_from_errno());
  return pos;
}

PInteger FILESIZE(const PFile* file) {
  check_ioresult();
  if (!is_open(file)) return 0;
  long prev = ftell(file->file);
  if (prev < 0) goto filesize_error;
  if (fseek(file->file, 0, SEEK_END) < 0) goto filesize_error;
  long size = ftell(file->file);
  if (size < 0) goto filesize_error;
  if (fseek(file->file, prev, SEEK_SET) < 0) goto filesize_error;
  return size;

filesize_error:
  set_ioresult(file, ioerror_from_errno());
  return 0;
}

void SEEK(PFile* file, PInteger pos) {
  check_ioresult();
  if (!is_open(file)) return;
  if (fseek(file->file, pos, SEEK_SET) < 0)
    set_ioresult(file, ioerror_from_errno());
}

void FLUSH(PFile* file) {
  check_ioresult();
  if (!is_open(file)) return;
  if (fflush(file->file) < 0) set_ioresult(file, ioerror_from_errno());
}

void ERASE(PFile* file) {
  check_ioresult();
  if (unlink(pchar_of_str(&file->name)) < 0)
    set_ioresult(file, ioerror_from_errno());
}

void RENAME(PFile* file, const PString* name) {
  char old_name[256];
  char new_name[256];
  check_ioresult();
  strcpy(old_name, pchar_of_str(&file->name));
  strcpy(new_name, pchar_of_str(name));
  if (rename(old_name, new_name) < 0)
    set_ioresult(file, ioerror_from_errno());
  else
    file->name = *name;
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

static PBoolean read_token(PFile* file, PString* str) {
  check_ioresult();
  if (!is_open(file)) return 0;
  clearerr(file->file);
  int len = 0;
  int chr = 0;
  enum { Blanks, Token, End } stage = Blanks;
  while (len < 255 && stage != 2) {
    chr = fgetc(file->file);
    if (chr == -1)
      break;
    else if (chr == ' ' || chr == '\t' || chr == '\n') {
      if (stage == Token) {
        stage = End;
        ungetc(chr, file->file);
      }
    } else {
      if (stage == Blanks) stage = Token;
      str->value[len++] = chr;
    }
  }
  str->len = len;
  if (ferror(file->file)) {
    set_ioresult(file, ieReadError);
    return 0;
  }
  if (chr == -1) return 0;
  return 1;
}

void READ_i(PFile* file, PInteger* num) {
  PString str;
  PInteger code;
  if (read_token(file, &str)) {
    VAL_i(&str, num, &code);
    if (code != 0) set_ioresult(file, ieReadError);
  }
}

void READ_r(PFile* file, PReal* num) {
  PString str;
  PInteger code;
  if (read_token(file, &str)) {
    VAL_r(&str, num, &code);
    if (code != 0) set_ioresult(file, ieReadError);
  }
}

void READ_c(PFile* file, PChar* chr) {
  int ch = 0;
  check_ioresult();
  if (!is_open(file)) return;
  clearerr(file->file);
  ch = fgetc(file->file);
  if (ferror(file->file))
    set_ioresult(file, ieReadError);
  else if (ch == -1)
    *chr = 26;
  else
    *chr = ch;
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

void WRITE_b(PFile* file, PBoolean val, ...) {
  PString str;
  STR_b(val, -1, &str);
  WRITE_s(file, str);
}

void WRITE_i(PFile* file, PInteger num, ...) {
  PString str;
  STR_i(num, -1, &str);
  WRITE_s(file, str);
}

void WRITE_r(PFile* file, PReal num, ...) {
  PString str;
  STR_r(num, -1, -1, &str);
  WRITE_s(file, str);
}

void WRITE_c(PFile* file, PChar chr, ...) {
  check_ioresult();
  if (!is_open(file)) return;
  clearerr(file->file);
  fputc(chr, file->file);
  if (ferror(file->file)) set_ioresult(file, ieWriteError);
}

void WRITE_s(PFile* file, PString str, ...) {
  check_ioresult();
  if (!is_open(file)) return;
  clearerr(file->file);
  for (int pos = 0; pos < str.len; ++pos) fputc(str.value[pos], file->file);
  if (ferror(file->file)) set_ioresult(file, ieWriteError);
}

void WRITE_e(PFile* file, POrdinal value, const char** names, ...) {
  PString str;
  STR_e(value, names, -1, &str);
  WRITE_s(file, str);
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