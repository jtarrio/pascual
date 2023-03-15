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

void CHDIR(const PString* dir);
void MKDIR(const PString* dir);
void RMDIR(const PString* dir);
void GETDIR(PInteger drive, PString* dir);

void ASSIGN(PFile* file, const PString* name);
void CLOSE(PFile* file);
void RESET(PFile* file);
void REWRITE(PFile* file);
PBoolean EOF(PFile* file);
PBoolean EOLN(PFile* file);
PBoolean SEEKEOF(PFile* file);
PBoolean SEEKEOLN(PFile* file);
PInteger FILEPOS(const PFile* file);
PInteger FILESIZE(const PFile* file);
void SEEK(PFile* file, PInteger pos);
void FLUSH(PFile* file);

void ERASE(PFile* file);
void RENAME(PFile* file, const PString* name);

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
void READ(PFile* file, ...);
void WRITE(PFile* file, ...);

void InitFile();

#endif  // __PASCUAL_FILE_H
