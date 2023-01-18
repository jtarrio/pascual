#ifndef __PASCUAL_FILE_H
#define __PASCUAL_FILE_H

#include "types.h"

#ifdef EOF
#undef EOF
#endif

extern PFile INPUT;
extern PFile OUTPUT;
extern PFile STDERR;

void ASSIGN(PFile* file, PString name);
void CLOSE(PFile* file);
void RESET(PFile* file);
void REWRITE(PFile* file);
PBoolean EOF(PFile* file);

void readln(PFile file);
void read_s(PFile file, PString* str);

void writeln(PFile file);
void write_b(PFile file, PBoolean val);
void write_i(PFile file, int num);
void write_c(PFile file, char chr);
void write_s(PFile file, PString str);
void write_e(PFile file, int value, const char** names);

void InitFile();

#endif  // __PASCUAL_FILE_H
