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
  if (file->handle != NULL) return 1;
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
  fclose(file->handle);
  file->handle = NULL;
}

PBoolean Eof(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 1;
  clearerr(file->handle);
  int ch = fgetc(file->handle);
  if (feof(file->handle)) return 1;
  if (ferror(file->handle)) set_ioresult(file, ieReadError);
  if (die_on_error) check_ioresult();
  ungetc(ch, file->handle);
  return 0;
}

PBoolean Seekeof(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 1;
  clearerr(file->handle);
  int ch;
  do {
    ch = fgetc(file->handle);
    if (feof(file->handle)) return 1;
  } while (ch == ' ' || ch == '\t');
  if (ferror(file->handle)) set_ioresult(file, ieReadError);
  if (die_on_error) check_ioresult();
  ungetc(ch, file->handle);
  return 0;
}

PBoolean Eoln(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 1;
  clearerr(file->handle);
  int ch = fgetc(file->handle);
  if (feof(file->handle)) return 1;
  if (ferror(file->handle)) set_ioresult(file, ieReadError);
  if (die_on_error) check_ioresult();
  ungetc(ch, file->handle);
  return ch == '\n';
}

PBoolean Seekeoln(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 1;
  clearerr(file->handle);
  int ch;
  do {
    ch = fgetc(file->handle);
    if (feof(file->handle)) return 1;
  } while (ch == ' ' || ch == '\t');
  if (ferror(file->handle)) set_ioresult(file, ieReadError);
  if (die_on_error) check_ioresult();
  ungetc(ch, file->handle);
  return ch == '\n';
}

PInteger Filepos(const PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 0;
  PInteger pos = ftell(file->handle);
  if (pos < 0) set_ioresult(file, ioerror_from_errno());
  if (die_on_error) check_ioresult();
  return pos / file->block_size;
}

PInteger Filesize(const PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 0;
  long prev = ftell(file->handle);
  if (prev < 0) goto filesize_error;
  if (fseek(file->handle, 0, SEEK_END) < 0) goto filesize_error;
  long size = ftell(file->handle);
  if (size < 0) goto filesize_error;
  if (fseek(file->handle, prev, SEEK_SET) < 0) goto filesize_error;
  return size / file->block_size;

filesize_error:
  set_ioresult(file, ioerror_from_errno());
  if (die_on_error) check_ioresult();
  return 0;
}

void Seek(PFile* file, PInteger pos, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  if (fseek(file->handle, pos * file->block_size, SEEK_SET) < 0)
    set_ioresult(file, ioerror_from_errno());
  if (die_on_error) check_ioresult();
}

void Flush(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  if (fflush(file->handle) < 0) set_ioresult(file, ioerror_from_errno());
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

static inline void open_file(PFile* file, const char* mode, PInteger block_size,
                             PBoolean die_on_error) {
  check_ioresult();
  if (file->handle != NULL) {
    fclose(file->handle);
    file->handle = NULL;
  }
  file->handle = fopen(pchar_of_str(&file->name), mode);
  if (file->handle == NULL) {
    set_ioresult(file, errno == ENOENT   ? ieFileNotFound
                       : errno == EACCES ? ieAccessDenied
                                         : ieUnknown);
  }
  file->block_size = block_size < 1 ? 1 : block_size;
  if (die_on_error) check_ioresult();
}

void Reset(PFile* file, PInteger block_size, PBoolean die_on_error) {
  const char* mode = block_size <= 0 ? "r" : "r+";
  open_file(file, mode, block_size, die_on_error);
}

void Rewrite(PFile* file, PInteger block_size, PBoolean die_on_error) {
  const char* mode = block_size <= 0 ? "w" : "w+";
  open_file(file, mode, block_size, die_on_error);
}

static void readln(PFile* file, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  clearerr(file->handle);
  int chr;
  do {
    chr = fgetc(file->handle);
  } while ((chr != '\n') && (chr != -1));
  if (ferror(file->handle)) set_ioresult(file, ieReadError);
  if (die_on_error) check_ioresult();
}

static PBoolean read_token(PFile* file, PString* str, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return 0;
  clearerr(file->handle);
  int len = 0;
  int chr = 0;
  enum { Blanks, Token, End } stage = Blanks;
  while (len < 255 && stage != 2) {
    chr = fgetc(file->handle);
    if (chr == -1)
      break;
    else if (chr == ' ' || chr == '\t' || chr == '\n') {
      if (stage == Token) {
        stage = End;
        ungetc(chr, file->handle);
      }
    } else {
      if (stage == Blanks) stage = Token;
      str->value[len++] = chr;
    }
  }
  str->len = len;
  if (ferror(file->handle)) {
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
  clearerr(file->handle);
  ch = fgetc(file->handle);
  if (ferror(file->handle))
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
  clearerr(file->handle);
  int chr = 0;
  int len = 0;
  while ((len < 255) && (chr != '\n')) {
    chr = fgetc(file->handle);
    if (chr == -1)
      break;
    else if (chr == '\n')
      ungetc(chr, file->handle);
    else
      str->value[len++] = chr;
  }
  str->len = len;
  if (ferror(file->handle)) set_ioresult(file, ieReadError);
  if (die_on_error) check_ioresult();
}

static void read_dataptr(PFile* file, void* ptr, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  clearerr(file->handle);
  fread(ptr, file->block_size, 1, file->handle);
  if (ferror(file->handle)) set_ioresult(file, ieReadError);
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
      case RwpDataPtr: {
        read_dataptr(file, va_arg(args, void*), die_on_error);
        break;
      }
    }
    if (error_code != 0) break;
    if (paramtype & RwpLn) readln(file, die_on_error);
  } while ((paramtype & RwpEnd) == 0);
  va_end(args);
}

static void writestr(PFile* file, int strlen, const PChar* strptr, int width,
                     int linefeed, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  clearerr(file->handle);
  for (int i = 0; i < width - strlen; ++i) fputc(' ', file->handle);
  for (int pos = 0; pos < strlen; ++pos) fputc(strptr[pos], file->handle);
  if (linefeed) fputc('\n', file->handle);
  if (ferror(file->handle)) set_ioresult(file, ieWriteError);
  if (die_on_error) check_ioresult();
}

static void writedata(PFile* file, const void* ptr, PBoolean die_on_error) {
  check_ioresult();
  if (!is_open(file, die_on_error)) return;
  clearerr(file->handle);
  fwrite(ptr, file->block_size, 1, file->handle);
  if (ferror(file->handle)) set_ioresult(file, ieWriteError);
  if (die_on_error) check_ioresult();
}

void Write(PFile* file, PBoolean die_on_error, ...) {
  struct {
    enum { Str, StrPtr, DataPtr } type;
    union {
      PString str;
      struct {
        int strlen;
        const PChar* strptr;
      };
      const void* ptr;
    };
  } data;
  enum ReadWriteParamType paramtype;
  va_list args;
  va_start(args, die_on_error);
  do {
    data.type = Str;
    data.str.len = 0;
    paramtype = va_arg(args, enum ReadWriteParamType);
    int width = paramtype & RwpWidth ? va_arg(args, int) : 0;
    int prec = paramtype & RwpPrec ? va_arg(args, int) : -1;
    switch (paramtype & 0x0f) {
      case RwpBool:
        STR_b(va_arg(args, PBoolean), 0, &data.str);
        break;
      case RwpInt:
        STR_i(va_arg(args, PInteger), width, &data.str);
        break;
      case RwpReal:
        STR_r(va_arg(args, PReal), width, prec, &data.str);
        break;
      case RwpChar:
        data.str.value[0] = va_arg(args, int);
        data.str.len = 1;
        break;
      case RwpEnum: {
        POrdinal ordinal = va_arg(args, POrdinal);
        const char** enumvalues = va_arg(args, const char**);
        STR_e(ordinal, enumvalues, 0, &data.str);
        break;
      }
      case RwpString:
        data.str = va_arg(args, PString);
        break;
      case RwpStringPtr: {
        data.type = StrPtr;
        PString* dst = va_arg(args, PString*);
        data.strlen = dst->len;
        data.strptr = dst->value;
        break;
      }
      case RwpLenPtr: {
        data.type = StrPtr;
        data.strlen = va_arg(args, int);
        data.strptr = va_arg(args, const PChar*);
        break;
      }
      case RwpDataPtr: {
        data.type = DataPtr;
        data.ptr = va_arg(args, const void*);
      }
    }
    switch (data.type) {
      case Str:
        writestr(file, data.str.len, data.str.value, width, paramtype & RwpLn,
                 die_on_error);
        break;
      case StrPtr:
        writestr(file, data.strlen, data.strptr, width, paramtype & RwpLn,
                 die_on_error);
        break;
      case DataPtr:
        writedata(file, data.ptr, die_on_error);
        break;
    }
    if (error_code != 0) break;
  } while ((paramtype & RwpEnd) == 0);
  va_end(args);
}

PFile INPUT = {};
PFile OUTPUT = {};
PFile STDERR = {};

void InitFile() {
  error_code = 0;
  error_file = NULL;
  INPUT.handle = stdin;
  INPUT.name = str_make(7, "(stdin)");
  OUTPUT.handle = stdout;
  OUTPUT.name = str_make(8, "(stdout)");
  STDERR.handle = stderr;
  STDERR.name = str_make(8, "(stderr)");
}
