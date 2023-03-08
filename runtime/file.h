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

void READLN(PFile* file);
void READ_i(PFile* file, PInteger* num);
void READ_r(PFile* file, PReal* num);
void READ_c(PFile* file, PChar* chr);
void READ_s(PFile* file, PString* str);

void WRITELN(PFile* file);
void WRITE_b(PFile* file, PBoolean val, PInteger width);
void WRITE_i(PFile* file, PInteger num, PInteger width);
void WRITE_r(PFile* file, PReal num, PInteger width, PInteger precision);
void WRITE_c(PFile* file, PChar chr, PInteger width);
void WRITE_s(PFile* file, PString str, PInteger width);
void WRITE_e(PFile* file, POrdinal value, const char** names, PInteger width);

void InitFile();

#endif  // __PASCUAL_FILE_H
