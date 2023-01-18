#ifndef __PASCUAL_STRING_H
#define __PASCUAL_STRING_H

#include "types.h"

int LENGTH(PString s);
PString COPY(PString src, int pos, int num);
void DELETE(PString* src, int pos, int num);
char UPCASE(char src);
PString str_of(char chr);
#define str_make(l, s) \
  (PString) { .len = l, .value = s }
PString str_of_pchar(const char* str);

PString cat_cc(char a, char b);
PString cat_cs(char a, PString b);
PString cat_sc(PString a, char b);
PString cat_ss(PString a, PString b);

int cmp_cc(char a, char b);
int cmp_cs(char a, PString b);
int cmp_sc(PString a, char b);
int cmp_ss(PString a, PString b);

PString to_str_b(PBoolean val);
PString to_str_i(int num);
PString to_str_c(char chr);
PString to_str_s(PString str);
PString to_str_e(int value, const char** names);

#endif  // __PASCUAL_STRING_H
