#ifndef __PASCUAL_STRING_H
#define __PASCUAL_STRING_H

#include "types.h"

#define LENGTH(s) (s)->len
PString COPY(const PString* src, PInteger pos, PInteger num);
void DELETE(PString* src, PInteger pos, PInteger num);
void INSERT(const PString* ins, PString* target, PInteger pos);
PInteger POS(const PString* needle, const PString* haystack);
PChar UPCASE(PChar src);
PChar CHR(PInteger pos);
PInteger ORD(PChar chr);

#define str_make(l, s) \
  (PString) { .len = l, .value = s }
#define str_of(c) str_make(1, {c})
PString str_of_pchar(const char* str, PInteger width);
const char* pchar_of_str(const PString* str);

enum ConcatParamType {
  CpChar = 0x01,
  CpString = 0x02,
  CpStringPtr = 0x03,
  CpLenPtr = 0x04,
  CpEnd = 0x80
};
PString CONCAT(enum ConcatParamType paramtype, ...);

PString cat_cc(PChar a, PChar b);
PString cat_cs(PChar a, PString b);
PString cat_sc(PString a, PChar b);
PString cat_ss(PString a, PString b);

PInteger cmp_cc(PChar a, PChar b);
PInteger cmp_cs(PChar a, PString b);
PInteger cmp_sc(PString a, PChar b);
PInteger cmp_ss(PString a, PString b);

void STR_b(PBoolean val, PInteger width, PString* dst);
void STR_i(PInteger num, PInteger width, PString* dst);
void STR_r(double num, PInteger width, PInteger precision, PString* dst);
void STR_e(POrdinal value, const char** names, PInteger width, PString* dst);

void VAL_b(const PString* str, PBoolean* dst, PInteger* code);
void VAL_i(const PString* str, PInteger* dst, PInteger* code);
void VAL_r(const PString* str, double* dst, PInteger* code);
void VAL_e(const PString* str, void* dst, PInteger num_names,
           const char** names, PInteger* code);

#endif  // __PASCUAL_STRING_H
