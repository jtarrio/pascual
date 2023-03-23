#include "file.h"

#include <errno.h>
#include <stdarg.h>
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

void Chdir(const PString* dir, PBoolean die_on_error) {
  check_ioresult();
  if (!chdir(pchar_of_str(dir))) set_ioresult(NULL, ioerror_from_errno());
  if (die_on_error) check_ioresult();
}

void Mkdir(const PString* dir, PBoolean die_on_error) {
  check_ioresult();
  if (!mkdir(pchar_of_str(dir), 0755)) set_ioresult(NULL, ioerror_from_errno());
  if (die_on_error) check_ioresult();
}

void Rmdir(const PString* dir, PBoolean die_on_error) {
  check_ioresult();
  if (!rmdir(pchar_of_str(dir))) set_ioresult(NULL, ioerror_from_errno());
  if (die_on_error) check_ioresult();
}

void Getdir(PInteger drive, PString* dir, PBoolean die_on_error) {
  char buf[256];
  size_t buf_size = 256;
  check_ioresult();
  if (getcwd(buf, buf_size) == NULL)
    set_ioresult(NULL, ioerror_from_errno());
  else
    *dir = str_of_pchar(buf, 0);
  if (die_on_error) check_ioresult();
}

static inline PBoolean is_open(const PFile* file, PBoolean die_on_error) {
  if (file->file != NULL) return 1;
  set_ioresult(file, ieFileNotOpen);
  if (die_on_error) check_ioresult();
  return 0;
}

void Assign(PFile* file, const PString* name, PBoolean die_on_error) {
  check_ioresult();
  file->name = *name;
}

void Close(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  fclose(file->file);
  file->file = NULL;
}

PBoolean Eof(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 1;
  clearerr(file->file);
  int ch = fgetc(file->file);
  if (feof(file->file)) return 1;
  if (ferror(file->file)) set_ioresult(file, ieReadError);
  if (die_on_error) check_ioresult();
  ungetc(ch, file->file);
  return 0;
}

PBoolean Seekeof(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 1;
  clearerr(file->file);
  int ch;
  do {
    ch = fgetc(file->file);
    if (feof(file->file)) return 1;
  } while (ch == ' ' || ch == '\t');
  if (ferror(file->file)) set_ioresult(file, ieReadError);
  if (die_on_error) check_ioresult();
  ungetc(ch, file->file);
  return 0;
}

PBoolean Eoln(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 1;
  clearerr(file->file);
  int ch = fgetc(file->file);
  if (feof(file->file)) return 1;
  if (ferror(file->file)) set_ioresult(file, ieReadError);
  if (die_on_error) check_ioresult();
  ungetc(ch, file->file);
  return ch == '\n';
}

PBoolean Seekeoln(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 1;
  clearerr(file->file);
  int ch;
  do {
    ch = fgetc(file->file);
    if (feof(file->file)) return 1;
  } while (ch == ' ' || ch == '\t');
  if (ferror(file->file)) set_ioresult(file, ieReadError);
  if (die_on_error) check_ioresult();
  ungetc(ch, file->file);
  return ch == '\n';
}

PInteger Filepos(const PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 0;
  PInteger pos = ftell(file->file);
  if (pos < 0) set_ioresult(file, ioerror_from_errno());
  if (die_on_error) check_ioresult();
  return pos;
}

PInteger Filesize(const PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 0;
  long prev = ftell(file->file);
  if (prev < 0) goto filesize_error;
  if (fseek(file->file, 0, SEEK_END) < 0) goto filesize_error;
  long size = ftell(file->file);
  if (size < 0) goto filesize_error;
  if (fseek(file->file, prev, SEEK_SET) < 0) goto filesize_error;
  return size;

filesize_error:
  set_ioresult(file, ioerror_from_errno());
  if (die_on_error) check_ioresult();
  return 0;
}

void Seek(PFile* file, PInteger pos, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  if (fseek(file->file, pos, SEEK_SET) < 0)
    set_ioresult(file, ioerror_from_errno());
  if (die_on_error) check_ioresult();
}

void Flush(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  if (fflush(file->file) < 0) set_ioresult(file, ioerror_from_errno());
  if (die_on_error) check_ioresult();
}

void Erase(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (unlink(pchar_of_str(&file->name)) < 0)
    set_ioresult(file, ioerror_from_errno());
  if (die_on_error) check_ioresult();
}

void Rename(PFile* file, const PString* name, PBoolean die_on_error) {
  char old_name[256];
  char new_name[256];
  check_ioresult();
  strcpy(old_name, pchar_of_str(&file->name));
  strcpy(new_name, pchar_of_str(name));
  if (rename(old_name, new_name) < 0)
    set_ioresult(file, ioerror_from_errno());
  else
    file->name = *name;
  if (die_on_error) check_ioresult();
}

static inline void open_file(PFile* file, const char* mode,
                             PBoolean die_on_error) {
  check_ioresult();
  file->file = fopen(pchar_of_str(&file->name), mode);
  if (file->file == NULL) {
    set_ioresult(file, errno == ENOENT   ? ieFileNotFound
                       : errno == EACCES ? ieAccessDenied
                                         : ieUnknown);
  }
  if (die_on_error) check_ioresult();
}

void Reset(PFile* file, PInteger block_size, PBoolean die_on_error) {
  open_file(file, "r", die_on_error);
}

void Rewrite(PFile* file, PInteger block_size, PBoolean die_on_error) {
  open_file(file, "w", die_on_error);
}

static void readln(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  clearerr(file->file);
  int chr;
  do {
    chr = fgetc(file->file);
  } while ((chr != '\n') && (chr != -1));
  if (ferror(file->file)) set_ioresult(file, ieReadError);
  if (die_on_error) check_ioresult();
}

static PBoolean read_token(PFile* file, PString* str, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 0;
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
    if (die_on_error) check_ioresult();
    return 0;
  }
  if (chr == -1) return 0;
  return 1;
}

static void read_char(PFile* file, PChar* chr, PBoolean die_on_error) {
  int ch = 0;
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  clearerr(file->file);
  ch = fgetc(file->file);
  if (ferror(file->file))
    set_ioresult(file, ieReadError);
  else if (ch == -1)
    *chr = 26;
  else
    *chr = ch;
  if (die_on_error) check_ioresult();
}

static void read_str(PFile* file, PString* str, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
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
  if (die_on_error) check_ioresult();
}

void Read(PFile* file, PBoolean die_on_error, ...) {
  PString str;
  int code = 0;
  enum ReadWriteParamType paramtype;
  va_list args;
  va_start(args, die_on_error);
  do {
    paramtype = va_arg(args, enum ReadWriteParamType);
    switch (paramtype & 0x0f) {
      case RwpInt:
        if (read_token(file, &str, die_on_error)) {
          VAL_i(&str, va_arg(args, PInteger*), &code);
          if (code != 0) set_ioresult(file, ieReadError);
        }
        break;
      case RwpReal:
        if (read_token(file, &str, die_on_error)) {
          VAL_r(&str, va_arg(args, PReal*), &code);
          if (code != 0) set_ioresult(file, ieReadError);
        }
        break;
      case RwpChar:
        read_char(file, va_arg(args, PChar*), die_on_error);
        break;
      case RwpString:
        read_str(file, va_arg(args, PString*), die_on_error);
        break;
    }
    if (paramtype & RwpLn) readln(file, die_on_error);
  } while ((paramtype & RwpEnd) == 0);
  va_end(args);
}

static void writestr(PFile* file, int strlen, const char* strptr, int width,
                     int linefeed, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  clearerr(file->file);
  for (int i = 0; i < width - strlen; ++i) fputc(' ', file->file);
  for (int pos = 0; pos < strlen; ++pos) fputc(strptr[pos], file->file);
  if (linefeed) fputc('\n', file->file);
  if (ferror(file->file)) set_ioresult(file, ieWriteError);
  if (die_on_error) check_ioresult();
}

void Write(PFile* file, PBoolean die_on_error, ...) {
  int use_ptr;
  PString str;
  int strlen;
  const char* strptr;
  enum ReadWriteParamType paramtype;
  va_list args;
  va_start(args, die_on_error);
  do {
    use_ptr = 0;
    str.len = 0;
    paramtype = va_arg(args, enum ReadWriteParamType);
    int width = paramtype & RwpWidth ? va_arg(args, int) : 0;
    int prec = paramtype & RwpPrec ? va_arg(args, int) : -1;
    switch (paramtype & 0x0f) {
      case RwpBool:
        STR_b(va_arg(args, PBoolean), 0, &str);
        break;
      case RwpInt:
        STR_i(va_arg(args, PInteger), width, &str);
        break;
      case RwpReal:
        STR_r(va_arg(args, PReal), width, prec, &str);
        break;
      case RwpChar:
        str.value[0] = va_arg(args, int);
        str.len = 1;
        break;
      case RwpEnum: {
        POrdinal ordinal = va_arg(args, POrdinal);
        const char** enumvalues = va_arg(args, const char**);
        STR_e(ordinal, enumvalues, 0, &str);
        break;
      }
      case RwpString:
        str = va_arg(args, PString);
        break;
      case RwpStringPtr: {
        PString* dst = va_arg(args, PString*);
        use_ptr = 1;
        strlen = dst->len;
        strptr = dst->value;
        break;
      }
      case RwpLenPtr: {
        use_ptr = 1;
        strlen = va_arg(args, int);
        strptr = va_arg(args, const char*);
        break;
      }
    }
    if (use_ptr)
      writestr(file, strlen, strptr, width, paramtype & RwpLn, die_on_error);
    else
      writestr(file, str.len, str.value, width, paramtype & RwpLn,
               die_on_error);
  } while ((paramtype & RwpEnd) == 0);
  va_end(args);
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
