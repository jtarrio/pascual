#ifndef __PASCUAL_H
#define __PASCUAL_H

#include <stdio.h>

#ifdef EOF
#undef EOF
#endif

void HALT(int code);

typedef int PBoolean;
extern const PBoolean FALSE;
extern const PBoolean TRUE;

typedef struct {
  union {
    char chr[256];
    struct __attribute__((__packed__)) {
      unsigned char len;
      char value[255];
    };
  };
} STRING;

int LENGTH(STRING s);
STRING COPY(STRING src, int pos, int num);
void DELETE(STRING* src, int pos, int num);
char UPCASE(char src);
STRING str_of(char chr);
#define str_make(l,s) (STRING){.len=l, .value=s}
STRING cat_cc(char a, char b);
STRING cat_cs(char a, STRING b);
STRING cat_sc(STRING a, char b);
STRING cat_ss(STRING a, STRING b);
int cmp_cc(char a, char b);
int cmp_cs(char a, STRING b);
int cmp_sc(STRING a, char b);
int cmp_ss(STRING a, STRING b);

STRING to_str_b(PBoolean val);
STRING to_str_i(int num);
STRING to_str_c(char chr);
STRING to_str_s(STRING str);
STRING to_str_e(int value, const char** names);

typedef struct {
  FILE* file;
} PFile;

extern PFile INPUT;
extern PFile OUTPUT;
extern PFile STDERR;

PBoolean EOF(PFile file);

void readln(PFile file);
void read_s(PFile file, STRING* str);

void writeln(PFile file);
void write_b(PFile file, PBoolean val);
void write_i(PFile file, int num);
void write_c(PFile file, char chr);
void write_s(PFile file, STRING str);
void write_e(PFile file, int value, const char** names);

#endif  // __PASCUAL_H
