#include "number.h"

int str_to_int(const PString* str, int* stop) {
  int neg = 0;
  int value = 0;
  *stop = 0;
  if (str->len > 1 && str->value[0] == '$') {
    for (int pos = 1; pos < str->len && *stop == 0; ++pos) {
      char chr = str->value[pos];
      if (chr >= '0' && chr <= '9')
        value = value * 16 + chr - '0';
      else if (chr >= 'a' && chr <= 'f')
        value = value * 16 + chr - 'a' + 10;
      else if (chr >= 'A' && chr <= 'F')
        value = value * 16 + chr - 'A' + 10;
      else
        *stop = pos + 1;
    }
    return value;
  }

  for (int pos = 0; pos < str->len && *stop == 0; ++pos) {
    char chr = str->value[pos];
    if (chr == '-' && pos == 0)
      neg = 1;
    else if (chr == '+' && pos == 0)
      neg = 0;
    else if (chr >= '0' && chr <= '9')
      value = value * 10 + chr - '0';
    else {
      *stop = pos + 1;
      return value;
    }
  }
  if (neg) value = -value;
  return value;
}

void int_to_str(int num, PString* str) {
  str->len = snprintf(str->value, 255, "%d", num);
}

double str_to_real(const PString* str, int* stop) {
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

  *stop = 0;
  for (int pos = 0; pos < str->len && *stop == 0; ++pos) {
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
      *stop = last + 1;
      return value;
    }
  }
  if (state != IntPart && state != FracPart && state != ScalePart) {
    *stop = last + 1;
    return value;
  }

  if (neg) value = -value;
  value = value / divisor;
  for (int i = 0; i < scale; ++i) {
    if (neg_scale)
      value = value / 10;
    else
      value = value * 10;
  }
  return value;
}

void real_to_str(double num, PString* str) {
  str->len = snprintf(str->value, 255, "%g", num);
}