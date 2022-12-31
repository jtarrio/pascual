#ifndef __RUNTIME_H
#define __RUNTIME_H

#include <stdio.h>

#define VAR

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

STRING str_of(char chr);
STRING str_make(int len, const char* str);
STRING str_concat_CHAR_CHAR(char a, char b);
STRING str_concat_CHAR_STRING(char a, STRING b);
STRING str_concat_STRING_CHAR(STRING a, char b);
STRING str_concat_STRING_STRING(STRING a, STRING b);
int str_compare_CHAR_CHAR(char a, char b);
int str_compare_CHAR_STRING(char a, STRING b);
int str_compare_STRING_CHAR(STRING a, char b);
int str_compare_STRING_STRING(STRING a, STRING b);

typedef struct {
  FILE* file;
} PFile;

extern PFile INPUT;
extern PFile OUTPUT;
extern PFile STDERR;

void InitFiles();

void readln(PFile file);
void read_STRING(PFile file, STRING* str);

void writeln(PFile file);
void write_BOOLEAN(PFile file, PBoolean val);
void write_INTEGER(PFile file, int num);
void write_CHAR(PFile file, char chr);
void write_STRING(PFile file, STRING str);

#endif  // __RUNTIME_H
