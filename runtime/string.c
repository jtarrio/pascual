#include "string.h"

#include <stdarg.h>
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

PChar UPCASE(PChar src) {
  if (src >= 'a' && src <= 'z') return src - ('a' - 'A');
  return src;
}

PChar LOWERCASE(PChar src) {
  if (src >= 'A' && src <= 'Z') return src + ('a' - 'A');
  return src;
}

PChar CHR(PInteger pos) { return pos; }

PInteger ORD(PChar chr) { return chr; }

PString str_of_pchar(const char* str, PInteger width) {
  PString ret;
  int length = 0;
  for (length = 0; length < 255; ++length) {
    if (str[length] == 0) break;
  }
  if (width > 255) width = 255;
  int spaces = width - length;
  if (spaces < 0) spaces = 0;
  for (int i = 0; i < spaces; ++i) {
    ret.value[i] = ' ';
  }
  for (int i = 0; i < length; ++i) {
    ret.value[i + spaces] = str[i];
  }
  ret.len = spaces + length;
  return ret;
}

const char* pchar_of_str(const PString* str) {
  static char buffer[256];
  memcpy(buffer, str->value, str->len);
  buffer[str->len] = 0;
  return buffer;
}

static void str_concat(PString* dst, int len, const PChar* src) {
  if (len > (255 - dst->len)) len = 255 - dst->len;
  if (len > 0) {
    memcpy(dst->value + dst->len, src, len);
    dst->len += len;
  }
}

PString CONCAT(enum ConcatCmpParamType paramtype, ...) {
  int end = 0;
  PString ret;
  ret.len = 0;
  va_list args;
  va_start(args, paramtype);
  do {
    end = paramtype & CpEnd;
    if (ret.len == 255) break;
    switch (paramtype & 0x0f) {
      case CpChar:
        ret.value[ret.len] = va_arg(args, int);
        ++ret.len;
        break;
      case CpString: {
        PString b = va_arg(args, PString);
        str_concat(&ret, b.len, b.value);
        break;
      }
      case CpStringPtr: {
        PString* b = va_arg(args, PString*);
        str_concat(&ret, b->len, b->value);
        break;
      }
      case CpLenPtr: {
        int cp = va_arg(args, int);
        const PChar* b = va_arg(args, const PChar*);
        str_concat(&ret, cp, b);
        break;
      }
    }
    paramtype = va_arg(args, enum ConcatCmpParamType);
  } while (!end);
  va_end(args);
  return ret;
}

struct StrData {
  PString str;
  int len;
  const PChar* ptr;
};

void LoadStrData(va_list* args, struct StrData* strdata) {
  enum ConcatCmpParamType type = va_arg(*args, enum ConcatCmpParamType);
  switch (type & 0x0f) {
    case CpChar:
      strdata->str.len = 1;
      strdata->str.value[0] = va_arg(*args, int);
      strdata->len = 1;
      strdata->ptr = strdata->str.value;
      break;
    case CpString:
      strdata->str = va_arg(*args, PString);
      strdata->len = strdata->str.len;
      strdata->ptr = strdata->str.value;
      break;
    case CpStringPtr: {
      PString* strptr = va_arg(*args, PString*);
      strdata->len = strptr->len;
      strdata->ptr = strptr->value;
      break;
    }
    case CpLenPtr: {
      int len = va_arg(*args, int);
      const PChar* ptr = va_arg(*args, const PChar*);
      strdata->len = len;
      strdata->ptr = ptr;
      break;
    }
  }
}

PBoolean cmp_str(enum CmpOp op, ...) {
  va_list args;
  va_start(args, op);
  struct StrData astr, bstr;
  LoadStrData(&args, &astr);
  LoadStrData(&args, &bstr);
  if ((op == CoEq || op == CoNotEq) && astr.len != bstr.len) {
    return op == CoNotEq;
  }
  int res = 0;
  for (int pos = 0; pos < astr.len && pos < bstr.len && res == 0; ++pos) {
    res = astr.ptr[pos] - bstr.ptr[pos];
  }
  if (res != 0)
    return op == CoNotEq ||
           (res < 0 && (op == CoBefore || op == CoBeforeOrEq)) ||
           (res > 0 && (op == CoAfter || op == CoAfterOrEq));
  if (astr.len != bstr.len)
    return (astr.len < bstr.len && (op == CoBefore || op == CoBeforeOrEq)) ||
           (astr.len > bstr.len && (op == CoAfter || op == CoAfterOrEq));
  return op == CoEq || op == CoBeforeOrEq || op == CoAfterOrEq;
}

PInteger cmp_cc(PChar a, PChar b) { return a - b; }

PInteger cmp_cs(PChar a, PString b) {
  int c = cmp_cc(a, b.value[0]);
  if (c != 0) return c;
  return 1 - b.len;
}

PInteger cmp_sc(PString a, PChar b) {
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

void STR_b(PBoolean val, PInteger width, PString* dst) {
  const PString kTrueValue = str_make(4, "TRUE");
  const PString kFalseValue = str_make(5, "FALSE");
  const PString* copy_from = val ? &kTrueValue : &kFalseValue;
  if (width < copy_from->len) width = copy_from->len;
  int spaces = width - copy_from->len;
  for (int i = 0; i < spaces; ++i) {
    dst->value[i] = ' ';
  }
  for (int i = 0; i < copy_from->len; ++i) {
    dst->value[i + spaces] = copy_from->value[i];
  }
  dst->len = width;
}

void STR_i(PInteger num, PInteger width, PString* dst) {
  integer_to_str(num, dst, width);
}

void STR_r(double num, PInteger width, PInteger precision, PString* dst) {
  real_to_str(num, dst, width, precision);
}

void STR_e(POrdinal value, const char** names, PInteger width, PString* dst) {
  *dst = str_of_pchar(names[value], width);
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

void VAL_e(const PString* str, void* dst, PInteger num_names,
           const char** names, PInteger* code) {
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
