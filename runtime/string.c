#include "string.h"

#include <string.h>

#include "error.h"
#include "number.h"

void ClampStringBounds(const PString* src, PInteger* pos, PInteger* len) {
  if (*pos < 1 || *pos > 255) rt_error(reOutOfBounds);
  if (*pos > src->len) *pos = src->len + 1;
  if (*len < 1) *len = 0;
  PInteger max_len = 1 + src->len - *pos;
  if (*len > max_len) *len = max_len;
}

PString COPY(const PString* src, PInteger pos, PInteger num) {
  ClampStringBounds(src, &pos, &num);
  PString ret;
  if ((pos <= src->len) && (num > 0)) {
    ret.len = num;
    memcpy(ret.value, src->value + pos - 1, num);
  } else
    ret.len = 0;
  return ret;
}

void DELETE(PString* str, PInteger pos, PInteger num) {
  ClampStringBounds(str, &pos, &num);
  if ((pos <= str->len) && (num > 0)) {
    memmove(str->value + pos - 1, str->value + pos + num - 1, 255 - num);
  }
  str->len = str->len - num;
}

void INSERT(const PString* ins, PString* target, PInteger pos) {
  if (pos < 1 || pos > 255) rt_error(reOutOfBounds);
  if (pos > target->len) pos = target->len + 1;

  PInteger target_off = pos + ins->len - 1;
  PInteger src_off = pos - 1;
  PInteger copy_len = 255 - target_off;
  if (copy_len > 0)
    memmove(target->value + target_off, target->value + src_off, copy_len);
  copy_len = ins->len;
  if (copy_len + src_off > 255) copy_len = 255 - src_off;
  memmove(target->value + src_off, ins->value, ins->len);
  PInteger new_len = target->len + ins->len;
  if (new_len > 255) new_len = 255;
  target->len = new_len;
}

PInteger POS(const PString* needle, const PString* haystack) {
  if (needle->len == 0 || haystack->len == 0 || needle->len > haystack->len)
    return 0;
  for (int i = 0; i < haystack->len - needle->len; ++i) {
    PBoolean matches = 1;
    for (int j = 0; j < needle->len && matches; ++j) {
      if (needle->value[j] != haystack->value[i + j]) matches = 0;
    }
    if (matches) return i + 1;
  }
  return 0;
}

unsigned char UPCASE(unsigned char src) {
  if ((src >= 'a') && (src <= 'z')) return src - ('a' - 'A');
  return src;
}

unsigned char CHR(PInteger pos) { return pos; }

PInteger ORD(unsigned char chr) { return chr; }

PString str_of_pchar(const char* str) {
  PString ret;
  ret.len = strnlen(str, 255);
  memcpy(ret.value, str, ret.len);
  return ret;
}

const char* pchar_of_str(const PString* str) {
  static char buffer[256];
  memcpy(buffer, str->value, str->len);
  buffer[str->len] = 0;
  return buffer;
}

PString cat_cc(unsigned char a, unsigned char b) {
  PString ret;
  ret.len = 2;
  ret.value[0] = a;
  ret.value[1] = b;
  return ret;
}

PString cat_cs(unsigned char a, PString b) {
  memmove(b.value + 1, b.value, 254);
  b.value[0] = a;
  if (b.len < 255) ++b.len;
  return b;
}

PString cat_sc(PString a, unsigned char b) {
  if (a.len < 255) {
    a.value[a.len] = b;
    ++a.len;
  }
  return a;
}

PString cat_ss(PString a, PString b) {
  int cp = b.len;
  if (cp > (255 - a.len)) cp = 255 - a.len;
  memcpy(a.value + a.len, b.value, cp);
  a.len += cp;
  return a;
}

PInteger cmp_cc(unsigned char a, unsigned char b) { return a - b; }

PInteger cmp_cs(unsigned char a, PString b) {
  int c = cmp_cc(a, b.value[0]);
  if (c != 0) return c;
  return 1 - b.len;
}

PInteger cmp_sc(PString a, unsigned char b) {
  if (a.len == 0) return -1;
  if (a.len == 1) return cmp_cc(a.value[0], b);
  return 1;
}

PInteger cmp_ss(PString a, PString b) {
  for (int i = 0; (i < a.len) && (i < b.len); ++i) {
    PInteger c = cmp_cc(a.value[i], b.value[i]);
    if (c != 0) return c;
  }
  return a.len - b.len;
}

void STR_b(PBoolean val, PString* dst) {
  if (val)
    *dst = str_make(4, "TRUE");
  else
    *dst = str_make(5, "FALSE");
}

void STR_i(PInteger num, PString* dst) { integer_to_str(num, dst, -1); }

void STR_r(double num, PString* dst) { real_to_str(num, dst, -1, -1); }

void STR_c(unsigned char chr, PString* dst) {
  dst->len = 1;
  dst->value[0] = chr;
}

void STR_s(PString str, PString* dst) { *dst = str; }

void STR_e(POrdinal value, const char** names, PString* dst) {
  *dst = str_of_pchar(names[value]);
}

void VAL_b(const PString* str, PBoolean* dst, PInteger* code) {
  *code = 0;
  if (str->len == 4 && ((str->value[0] | 0x20) == 't') &&
      ((str->value[1] | 0x20) == 'r') && ((str->value[2] | 0x20) == 'u') &&
      ((str->value[3] | 0x20) == 'e'))
    *dst = 1;
  else if (str->len == 5 && ((str->value[0] | 0x20) == 'f') &&
           ((str->value[1] | 0x20) == 'a') && ((str->value[2] | 0x20) == 'l') &&
           ((str->value[3] | 0x20) == 's') && ((str->value[4] | 0x20) == 'e'))
    *dst = 0;
  else
    *code = 1;
}

void VAL_i(const PString* str, PInteger* dst, PInteger* code) {
  *dst = str_to_integer(str, code);
}

void VAL_r(const PString* str, double* dst, PInteger* code) {
  *dst = str_to_real(str, code);
}

void VAL_e(const PString* str, void* dst, PInteger num_names, const char** names,
           PInteger* code) {
  *code = 0;
  for (POrdinal e = 0; e < num_names; ++e) {
    PBoolean matches = 1;
    for (int i = 0; i < str->len && matches; ++i) {
      if ((str->value[i] & ~0x20) != (names[e][i])) matches = 0;
    }
    if (matches && names[e][str->len] == 0) {
      *(POrdinal*)dst = e;
      return;
    }
  }
  *code = 1;
}
