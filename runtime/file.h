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
  RwpDataPtr = 0x09,
  RwpWidth = 0x10,
  RwpPrec = 0x20,
  RwpLn = 0x40,
  RwpEnd = 0x80,
};
void Read(PFile* file, PBoolean die_on_error, ...);
void Write(PFile* file, PBoolean die_on_error, ...);

void InitFile();

#endif  // __PASCUAL_FILE_H
