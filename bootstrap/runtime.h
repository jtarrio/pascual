#ifndef __RUNTIME_H
#define __RUNTIME_H

#include <stdio.h>

#define VAR

typedef int PBoolean;
extern const PBoolean PFalse;
extern const PBoolean PTrue;

typedef struct {
  union {
    char chr[256];
    struct __attribute__((__packed__)) {
      char len;
      char value[255];
    };
  };
} PString;

#define PSTRING(str) {.len=sizeof(str)-1, .value=(str)}
#define STRLEN(str) ((str).chr[0])
#define STRELEM(str, i) ((str).chr[i])
#define STRLEN_MAX 255

void Pclear(VAR PString* str);
void Pappendchar(VAR PString* str, char chr);
PString Pcopy(const PString* str, int pos, int num);
void Pdelete(VAR PString* str, int pos, int num);
int Plength(const PString* str);
int Pcompare(const PString* a, const PString* b);

typedef struct {
  FILE* file;
} PFile;

extern PFile Pinput;
extern PFile Poutput;
void InitFiles();

void Pread_string(PFile file, VAR PString* var);
void Preadln(PFile file);
void Pwrite_integer(PFile file, int var);
void Pwrite_string(PFile file, const PString* var);
void Pwriteln(PFile file);
PBoolean Peof(PFile file);

#endif  // __RUNTIME_H
