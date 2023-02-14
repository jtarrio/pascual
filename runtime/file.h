#ifndef __PASCUAL_FILE_H
#define __PASCUAL_FILE_H

#include "types.h"

#ifdef EOF
#undef EOF
#endif

extern PFile INPUT;
extern PFile OUTPUT;
extern PFile STDERR;

int IORESULT();

void ASSIGN(PFile* file, const PString* name);
void CLOSE(PFile* file);
void RESET(PFile* file);
void REWRITE(PFile* file);
int EOF(PFile* file);

void READLN(PFile* file);
void READ_s(PFile* file, PString* str);

void WRITELN(PFile* file);
void WRITE_b(PFile* file, int val);
void WRITE_i(PFile* file, int num);
void WRITE_r(PFile* file, double num);
void WRITE_c(PFile* file, unsigned char chr);
void WRITE_s(PFile* file, PString str);
void WRITE_e(PFile* file, int value, const char** names);

void InitFile();

#endif  // __PASCUAL_FILE_H
