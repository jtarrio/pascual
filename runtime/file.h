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

void CHDIR(const PString* dir);
void MKDIR(const PString* dir);
void RMDIR(const PString* dir);
void GETDIR(int drive, PString* dir);

void ASSIGN(PFile* file, const PString* name);
void CLOSE(PFile* file);
void RESET(PFile* file);
void REWRITE(PFile* file);
int EOF(PFile* file);
int EOLN(PFile* file);
int SEEKEOF(PFile* file);
int SEEKEOLN(PFile* file);
int FILEPOS(const PFile* file);
int FILESIZE(const PFile* file);
void SEEK(PFile* file, int pos);
void FLUSH(PFile* file);

void ERASE(PFile* file);
void RENAME(PFile* file, const PString* name);

void READLN(PFile* file);
void READ_i(PFile* file, int* num);
void READ_r(PFile* file, double* num);
void READ_c(PFile* file, unsigned char* chr);
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
