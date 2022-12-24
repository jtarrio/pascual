#include "runtime.h"

#include <stdio.h>
#include <string.h>

const PBoolean PFalse = 0;
const PBoolean PTrue = !PFalse;

void Pclear(VAR PString* str) {
  STRLEN(*str) = 0;
}

void Pappendchar(VAR PString* str, char chr) {
  if (STRLEN(*str) == STRLEN_MAX) return;
  int pos = STRLEN(*str) + 1;
  STRELEM(*str, pos) = chr;
  STRLEN(*str) = pos;
}

PString Pcopy(const PString* str, int pos, int num) {
  PString res;
  if (pos < 1 || pos > STRLEN(*str)) return res;
  if (num < 0) num = 0;
  if (num > 1 + STRLEN(*str) - pos) num = 1 + STRLEN(*str) - pos;
  memcpy(&STRELEM(res, 1), &STRELEM(*str, pos), num);
  STRLEN(res) = num;
  return res;
}


void Pdelete(VAR PString* str, int pos, int num) {
  if (pos < 1 || pos > STRLEN(*str)) return;
  if (num < 0) num = 0;
  if (num > 1 + STRLEN(*str) - pos) num = 1 + STRLEN(*str) - pos;
  memmove(&STRELEM(*str, pos), &STRELEM(*str, pos + num), STRLEN(*str) - num);
  STRLEN(*str) = STRLEN(*str) - num;
}

int Plength(const PString* str) {
  return STRLEN(*str);
}

int Pcompare(const PString* a, const PString* b) {
  int pos = 1;
  int cmp;
  while (pos <= STRLEN(*a) && pos <= STRLEN(*b)) {
    cmp = STRELEM(*a, pos) - STRELEM(*b, pos);
    if (cmp != 0) return cmp;
    pos = pos + 1;
  }
  return STRLEN(*a) - STRLEN(*b);
}

void InitFiles() {
  Pinput.file = stdin;
  Poutput.file = stdout;
}

PFile Pinput = {};
PFile Poutput = {};

void Pread_string(PFile file, VAR PString* var) {
  char chr;
  Pclear(var);
  while (STRLEN(*var) < STRLEN_MAX) {
    int res = fgetc(file.file);
    if (res == EOF) break;
    if ((res == '\n') || (res == '\r')) {
      ungetc(res, file.file);
      break;
    }
    Pappendchar(var, res);
  }
}

void Preadln(PFile file) {
  while (1) {
    int res = fgetc(file.file);
    if (res == EOF) return;
    if (res == '\n') break;
    if (res == '\r') break;
  }
  while (1) {
    int res = fgetc(file.file);
    if (res == '\n') continue;
    if (res == '\r') continue;
    ungetc(res, file.file);
    break;
  }
}

void Pwrite_integer(PFile file, int var) {
  fprintf(file.file, "%d", var);
}

void Pwrite_string(PFile file, const PString* var) {
  for (int pos = 1; pos <= STRLEN(*var); ++pos) {
    fputc(STRELEM(*var, pos), file.file);
  }
}

void Pwriteln(PFile file) {
  fputc('\n', file.file);
}

PBoolean Peof(PFile file) { return feof(file.file); }
