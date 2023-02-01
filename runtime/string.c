#include "string.h"

#include <string.h>

#include "error.h"

void ClampStringBounds(PString* src, int* pos, int* len) {
  if (*pos < 1) rt_error(reOutOfBounds);
  if (*pos > src->len) *pos = src->len + 1;
  if (*len < 1) *len = 0;
  int max_len = 1 + src->len - *pos;
  if (*len > max_len) *len = max_len;
}

int LENGTH(PString s) { return s.len; }

PString COPY(PString src, int pos, int num) {
  ClampStringBounds(&src, &pos, &num);
  PString ret;
  if ((pos <= src.len) && (num > 0)) {
    ret.len = num;
    memcpy(ret.value, src.value + pos - 1, num);
  }
  return ret;
}

void DELETE(PString* str, int pos, int num) {
  ClampStringBounds(str, &pos, &num);
  if ((pos <= str->len) && (num > 0)) {
    memmove(str->value + pos - 1, str->value + pos + num - 1, 255 - num);
  }
  str->len = str->len - num;
}

char UPCASE(char src) {
  if ((src >= 'a') && (src <= 'z')) return src - ('a' - 'A');
  return src;
}

char CHR(int pos) {
  return (char) pos;
}

int ORD(char chr) {
  return (int) ((unsigned char) chr);
}

PString str_of(char chr) {
  PString ret;
  ret.len = 1;
  ret.value[0] = chr;
  return ret;
}

PString str_of_pchar(const char* str) {
  PString ret;
  ret.len = strnlen(str, 255);
  memcpy(ret.value, str, ret.len);
  return ret;
}

const char* pchar_of_str(PString* str) {
  static char buffer[256];
  memcpy(buffer, str->value, str->len);
  buffer[str->len] = 0;
  return buffer;
}

PString cat_cc(char a, char b) {
  PString ret;
  ret.len = 2;
  ret.value[0] = a;
  ret.value[1] = b;
  return ret;
}

PString cat_cs(char a, PString b) {
  memmove(b.value + 1, b.value, 254);
  b.value[0] = a;
  if (b.len < 255) ++b.len;
  return b;
}

PString cat_sc(PString a, char b) {
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

int cmp_cc(char a, char b) { return (unsigned char)(a) - (unsigned char)(b); }

int cmp_cs(char a, PString b) {
  int c = cmp_cc(a, b.value[0]);
  if (c != 0) return c;
  return 1 - b.len;
}

int cmp_sc(PString a, char b) {
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

PString to_str_b(PBoolean val) {
  if (val)
    return str_make(4, "TRUE");
  else
    return str_make(5, "FALSE");
}

PString to_str_i(int num) {
  PString ret;
  ret.len = snprintf(ret.value, 255, "%d", num);
  return ret;
}

PString to_str_c(char chr) { return str_of(chr); }

PString to_str_s(PString str) { return str; }

PString to_str_e(int value, const char** names) {
  return str_of_pchar(names[value]);
}
