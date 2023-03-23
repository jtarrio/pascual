#ifndef __PASCUAL_FILE_H
#define __PASCUAL_FILE_H

#include "types.h"

#ifdef EOF
#undef EOF
#endif

extern PFile INPUT;
extern PFile OUTPUT;
extern PFile STDERR;

PInteger IORESULT();

void Chdir(const PString* dir, PBoolean die_on_error);
void Mkdir(const PString* dir, PBoolean die_on_error);
void Rmdir(const PString* dir, PBoolean die_on_error);
void Getdir(PInteger drive, PString* dir, PBoolean die_on_error);

void Assign(PFile* file, const PString* name, PBoolean die_on_error);
void Close(PFile* file, PBoolean die_on_error);
void Reset(PFile* file, PInteger block_size, PBoolean die_on_error);
void Rewrite(PFile* file, PInteger block_size, PBoolean die_on_error);
PBoolean Eof(PFile* file, PBoolean die_on_error);
PBoolean Eoln(PFile* file, PBoolean die_on_error);
PBoolean Seekeof(PFile* file, PBoolean die_on_error);
PBoolean Seekeoln(PFile* file, PBoolean die_on_error);
PInteger Filepos(const PFile* file, PBoolean die_on_error);
PInteger Filesize(const PFile* file, PBoolean die_on_error);
void Seek(PFile* file, PInteger pos, PBoolean die_on_error);
void Flush(PFile* file, PBoolean die_on_error);

void Erase(PFile* file, PBoolean die_on_error);
void Rename(PFile* file, const PString* name, PBoolean die_on_error);

enum ReadWriteParamType {
  RwpBool = 0x01,
  RwpInt = 0x02,
  RwpReal = 0x03,
  RwpChar = 0x04,
  RwpEnum = 0x05,
  RwpString = 0x06,
  RwpStringPtr = 0x07,
  RwpLenPtr = 0x08,
  RwpWidth = 0x10,
  RwpPrec = 0x20,
  RwpLn = 0x40,
  RwpEnd = 0x80,
};
void Read(PFile* file, PBoolean die_on_error, ...);
void Write(PFile* file, PBoolean die_on_error, ...);

void InitFile();

#define CHDIR(dir) Chdir(dir, 0)
#define MKDIR(dir) Mkdir(dir, 0)
#define RMDIR(dir) Rmdir(dir, 0)
#define GETDIR(drive, dir) Getdir(drive, dir, 0)

#define ASSIGN(file, name) Assign(file, name, 0)
#define CLOSE(file) Close(file, 0)
#define RESET(file) Reset(file, 0, 0)
#define REWRITE(file) Rewrite(file, 0, 0)
#define EOF(file) Eof(file, 0)
#define EOLN(file) Eoln(file, 0)
#define SEEKEOF(file) Seekeof(file, 0)
#define SEEKEOLN(file) Seekeoln(file, 0)
#define FILEPOS(file) Filepos(file, 0)
#define FILESIZE(file) Filesize(file, 0)
#define SEEK(file, pos) Seek(file, pos, 0)
#define FLUSH(file) Flush(file, 0)

#define ERASE(file) Erase(file, 0)
#define RENAME(file, name) Rename(file, name, 0)

#define READ(file, ...) Read(file, 0, __VA_ARGS__)
#define WRITE(file, ...) Write(file, 0, __VA_ARGS__)

#endif  // __PASCUAL_FILE_H
