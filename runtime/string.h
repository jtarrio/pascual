#ifndef __PASCUAL_STRING_H
#define __PASCUAL_STRING_H

#include "types.h"

int LENGTH(PString s);
PString COPY(PString src, int pos, int num);
void DELETE(PString* src, int pos, int num);
char UPCASE(char src);
char CHR(int pos);
int ORD(char chr);

PString str_of(char chr);
#define str_make(l, s) \
  (PString) { .len = l, .value = s }
PString str_of_pchar(const char* str);
const char* pchar_of_str(PString* str);

PString cat_cc(char a, char b);
PString cat_cs(char a, PString b);
PString cat_sc(PString a, char b);
PString cat_ss(PString a, PString b);

int cmp_cc(char a, char b);
int cmp_cs(char a, PString b);
int cmp_sc(PString a, char b);
int cmp_ss(PString a, PString b);

void STR_b(int val, PString* dst);
void STR_i(int num, PString* dst);
void STR_r(double num, PString* dst);
void STR_e(int value, const char** names, PString* dst);

void VAL_b(PString* str, int* dst, int* code);
void VAL_i(PString* str, int* dst, int* code);
void VAL_r(PString* str, double* dst, int* code);
void VAL_e(PString* str, void* dst, int num_names, const char** names,
           int* code);

#endif  // __PASCUAL_STRING_H
