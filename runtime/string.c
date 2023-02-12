#include "string.h"

#include <string.h>

#include "error.h"

void ClampStringBounds(PString* src, int* pos, int* len) {
  if (*pos < 1 || *pos > 255) rt_error(reOutOfBounds);
  if (*pos > src->len) *pos = src->len + 1;
  if (*len < 1) *len = 0;
  int max_len = 1 + src->len - *pos;
  if (*len > max_len) *len = max_len;
}

PString COPY(PString src, int pos, int num) {
  ClampStringBounds(&src, &pos, &num);
  PString ret;
  if ((pos <= src.len) && (num > 0)) {
    ret.len = num;
    memcpy(ret.value, src.value + pos - 1, num);
  } else
    ret.len = 0;
  return ret;
}

void DELETE(PString* str, int pos, int num) {
  ClampStringBounds(str, &pos, &num);
  if ((pos <= str->len) && (num > 0)) {
    memmove(str->value + pos - 1, str->value + pos + num - 1, 255 - num);
  }
  str->len = str->len - num;
}

void INSERT(PString ins, PString* target, int pos) {
  if (pos < 1 || pos > 255) rt_error(reOutOfBounds);
  if (pos > target->len) pos = target->len + 1;

  int target_off = pos + ins.len - 1;
  int src_off = pos - 1;
  int copy_len = 255 - target_off;
  if (copy_len > 0)
    memmove(target->value + target_off, target->value + src_off, copy_len);
  copy_len = ins.len;
  if (copy_len + src_off > 255) copy_len = 255 - src_off;
  memmove(target->value + src_off, ins.value, ins.len);
  int new_len = target->len + ins.len;
  if (new_len > 255) new_len = 255;
  target->len = new_len;
}

int POS(PString needle, PString haystack) {
  if (needle.len == 0 || haystack.len == 0 || needle.len > haystack.len)
    return 0;
  for (int i = 0; i < haystack.len - needle.len; ++i) {
    int matches = 1;
    for (int j = 0; j < needle.len && matches; ++j) {
      if (needle.value[j] != haystack.value[i + j]) matches = 0;
    }
    if (matches) return i + 1;
  }
  return 0;
}

unsigned char UPCASE(unsigned char src) {
  if ((src >= 'a') && (src <= 'z')) return src - ('a' - 'A');
  return src;
}

unsigned char CHR(int pos) { return pos; }

int ORD(unsigned char chr) { return chr; }

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

int cmp_cc(unsigned char a, unsigned char b) { return a - b; }

int cmp_cs(unsigned char a, PString b) {
  int c = cmp_cc(a, b.value[0]);
  if (c != 0) return c;
  return 1 - b.len;
}

int cmp_sc(PString a, unsigned char b) {
  if (a.len == 0) return -1;
  if (a.len == 1) return cmp_cc(a.value[0], b);
  return 1;
}

int cmp_ss(PString a, PString b) {
  for (int i = 0; (i < a.len) && (i < b.len); ++i) {
    int c = cmp_cc(a.value[i], b.value[i]);
    if (c != 0) return c;
  }
  return a.len - b.len;
}

void STR_b(int val, PString* dst) {
  if (val)
    *dst = str_make(4, "TRUE");
  else
    *dst = str_make(5, "FALSE");
}

void STR_i(int num, PString* dst) {
  dst->len = snprintf(dst->value, 255, "%d", num);
}

void STR_r(double num, PString* dst) {
  dst->len = snprintf(dst->value, 255, "%g", num);
}

void STR_c(unsigned char chr, PString* dst) {
  dst->len = 1;
  dst->value[0] = chr;
}

void STR_s(PString str, PString* dst) { *dst = str; }

void STR_e(int value, const char** names, PString* dst) {
  *dst = str_of_pchar(names[value]);
}

void VAL_b(const PString* str, int* dst, int* code) {
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

void VAL_i(const PString* str, int* dst, int* code) {
  int neg = 0;
  *code = 0;
  *dst = 0;
  if (str->len > 1 && str->value[0] == '$') {
    for (int pos = 1; pos < str->len && *code == 0; ++pos) {
      char chr = str->value[pos];
      if (chr >= '0' && chr <= '9')
        *dst = *dst * 16 + chr - '0';
      else if (chr >= 'a' && chr <= 'f')
        *dst = *dst * 16 + chr - 'a' + 10;
      else if (chr >= 'A' && chr <= 'F')
        *dst = *dst * 16 + chr - 'A' + 10;
      else
        *code = pos + 1;
    }
    return;
  }

  for (int pos = 0; pos < str->len && *code == 0; ++pos) {
    char chr = str->value[pos];
    if (chr == '-' && pos == 0)
      neg = 1;
    else if (chr == '+' && pos == 0)
      neg = 0;
    else if (chr >= '0' && chr <= '9')
      *dst = *dst * 10 + chr - '0';
    else {
      *code = pos + 1;
      return;
    }
  }
  if (neg) *dst = -*dst;
}

void VAL_r(const PString* str, double* dst, int* code) {
  double value = 0.0;
  double divisor = 1.0;
  int scale = 0;
  int neg = 0;
  int neg_scale = 0;
  int last = 0;
  enum {
    IntPart,
    FracDot,
    FracPart,
    ScaleDot,
    ScaleSign,
    ScalePart
  } state = IntPart;

  *code = 0;
  for (int pos = 0; pos < str->len && *code == 0; ++pos) {
    char chr = str->value[pos];
    if (chr == '-' && pos == 0)
      neg = 1;
    else if (chr == '+' && pos == 0)
      neg = 0;
    else if (chr == '.' && state == IntPart)
      state = FracDot;
    else if ((chr == 'e' || chr == 'E') &&
             (state == IntPart || state == FracPart))
      state = ScaleDot;
    else if (chr == '-' && state == ScaleDot) {
      neg_scale = 1;
      state = ScaleSign;
    } else if (chr == '+' && state == ScaleDot) {
      neg_scale = 0;
      state = ScaleSign;
    } else if (chr >= '0' && chr <= '9') {
      if (state == FracDot) state = FracPart;
      if (state == IntPart || state == FracPart) value = value * 10 + chr - '0';
      if (state == FracPart) divisor = divisor * 10;
      if (state == ScaleDot) state = ScalePart;
      if (state == ScaleSign) state = ScalePart;
      if (state == ScalePart) scale = scale * 10 + chr - '0';
      last = pos + 1;
    } else {
      *code = last + 1;
      return;
    }
  }
  if (state != IntPart && state != FracPart && state != ScalePart) {
    *code = last + 1;
    return;
  }

  if (neg) value = -value;
  value = value / divisor;
  for (int i = 0; i < scale; ++i) {
    if (neg_scale)
      value = value / 10;
    else
      value = value * 10;
  }
  *dst = value;
}

void VAL_e(const PString* str, void* dst, int num_names, const char** names,
           int* code) {
  *code = 0;
  for (int e = 0; e < num_names; ++e) {
    int matches = 1;
    for (int i = 0; i < str->len && matches; ++i) {
      if ((str->value[i] & ~0x20) != (names[e][i])) matches = 0;
    }
    if (matches && names[e][str->len] == 0) {
      *(int*)dst = e;
      return;
    }
  }
  *code = 1;
}
