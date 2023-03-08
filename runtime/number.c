#include "number.h"

#include "string.h"

PInteger str_to_integer(const PString* str, PInteger* stop) {
  PBoolean neg = 0;
  PInteger value = 0;
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

void integer_to_str(PInteger num, PString* str, PInteger width) {
  PBoolean neg = num < 0;
  if (neg) num = -num;
  int digits = 1;
  PInteger divisor = 1;
  PInteger numDiv10 = num / 10;
  while (numDiv10 >= divisor) {
    divisor *= 10;
    ++digits;
  }
  if (neg) ++digits;
  if (width > 255) width = 255;
  if (width < digits) width = digits;
  PInteger spaces = width - digits;
  PInteger pos = 0;
  for (PInteger i = 0; i < spaces; ++i) {
    str->value[pos++] = ' ';
  }
  if (neg) str->value[pos++] = '-';
  do {
    str->value[pos++] = '0' + num / divisor;
    num %= divisor;
    divisor /= 10;
  } while (divisor > 0);
  str->len = pos;
}

PReal str_to_real(const PString* str, PInteger* stop) {
  const unsigned long kMaxMantissa = ~9ul / 10;
  unsigned long mantissa = 0;
  int divisor = 0;
  int scale = 0;
  PBoolean neg = 0;
  PBoolean neg_scale = 0;
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
      if (state == IntPart) {
        if (mantissa <= kMaxMantissa)
          mantissa = mantissa * 10 + chr - '0';
        else
          --divisor;
      }
      if (state == FracPart) {
        if (mantissa <= kMaxMantissa) {
          mantissa = mantissa * 10 + chr - '0';
          ++divisor;
        }
      }
      if (state == ScaleDot) state = ScalePart;
      if (state == ScaleSign) state = ScalePart;
      if (state == ScalePart) scale = scale * 10 + chr - '0';
      last = pos + 1;
    } else {
      *stop = last + 1;
      return 0;
    }
  }
  if (state != IntPart && state != FracPart && state != ScalePart) {
    *stop = last + 1;
    return 0;
  }

  PReal value = mantissa;
  if (neg_scale) scale = -scale;
  scale -= divisor;
  while (scale >= 22) {
    value = value * 1e22;
    scale -= 22;
  }
  while (scale <= -22) {
    value = value / 1e22;
    scale += 22;
  }
  switch (scale) {
    case 21:
      value = value * 1e21;
      break;
    case 20:
      value = value * 1e20;
      break;
    case 19:
      value = value * 1e19;
      break;
    case 18:
      value = value * 1e18;
      break;
    case 17:
      value = value * 1e17;
      break;
    case 16:
      value = value * 1e16;
      break;
    case 15:
      value = value * 1e15;
      break;
    case 14:
      value = value * 1e14;
      break;
    case 13:
      value = value * 1e13;
      break;
    case 12:
      value = value * 1e12;
      break;
    case 11:
      value = value * 1e11;
      break;
    case 10:
      value = value * 1e10;
      break;
    case 9:
      value = value * 1e9;
      break;
    case 8:
      value = value * 1e8;
      break;
    case 7:
      value = value * 1e7;
      break;
    case 6:
      value = value * 1e6;
      break;
    case 5:
      value = value * 1e5;
      break;
    case 4:
      value = value * 1e4;
      break;
    case 3:
      value = value * 1e3;
      break;
    case 2:
      value = value * 1e2;
      break;
    case 1:
      value = value * 1e1;
      break;
    case -1:
      value = value / 1e1;
      break;
    case -2:
      value = value / 1e2;
      break;
    case -3:
      value = value / 1e3;
      break;
    case -4:
      value = value / 1e4;
      break;
    case -5:
      value = value / 1e5;
      break;
    case -6:
      value = value / 1e6;
      break;
    case -7:
      value = value / 1e7;
      break;
    case -8:
      value = value / 1e8;
      break;
    case -9:
      value = value / 1e9;
      break;
    case -10:
      value = value / 1e10;
      break;
    case -11:
      value = value / 1e11;
      break;
    case -12:
      value = value / 1e12;
      break;
    case -13:
      value = value / 1e13;
      break;
    case -14:
      value = value / 1e14;
      break;
    case -15:
      value = value / 1e15;
      break;
    case -16:
      value = value / 1e16;
      break;
    case -17:
      value = value / 1e17;
      break;
    case -18:
      value = value / 1e18;
      break;
    case -19:
      value = value / 1e19;
      break;
    case -20:
      value = value / 1e20;
      break;
    case -21:
      value = value / 1e21;
      break;
  }
  if (neg) value = -value;
  return value;
}

void real_to_str(PReal num, PString* str, PInteger width, PInteger precision) {
  if (width > 255) width = 255;
  if (width <= 0) width = 22;
  if (precision < 0) precision = 14;
  if (precision > 0 && precision + 8 > width) precision = width - 8;
  if (precision == 0 && precision + 7 > width) precision = width - 7;
  PInteger spaces = width - precision - 8;
  int exp = 0;
  PBoolean neg = num < 0;
  if (neg) num = -num;
  if (num != 0) {
    exp = 15;
    while (num >= 1e37) {
      num /= 1e22;
      exp += 22;
    }
    while (num < 1e-6) {
      num *= 1e22;
      exp -= 22;
    }
    if (num >= 1e36) {
      num /= 1e21;
      exp += 21;
    } else if (num >= 1e35) {
      num /= 1e20;
      exp += 20;
    } else if (num >= 1e34) {
      num /= 1e19;
      exp += 19;
    } else if (num >= 1e33) {
      num /= 1e18;
      exp += 18;
    } else if (num >= 1e32) {
      num /= 1e17;
      exp += 17;
    } else if (num >= 1e31) {
      num /= 1e16;
      exp += 16;
    } else if (num >= 1e30) {
      num /= 1e15;
      exp += 15;
    } else if (num >= 1e29) {
      num /= 1e14;
      exp += 14;
    } else if (num >= 1e28) {
      num /= 1e13;
      exp += 13;
    } else if (num >= 1e27) {
      num /= 1e12;
      exp += 12;
    } else if (num >= 1e26) {
      num /= 1e11;
      exp += 11;
    } else if (num >= 1e25) {
      num /= 1e10;
      exp += 10;
    } else if (num >= 1e24) {
      num /= 1e9;
      exp += 9;
    } else if (num >= 1e23) {
      num /= 1e8;
      exp += 8;
    } else if (num >= 1e22) {
      num /= 1e7;
      exp += 7;
    } else if (num >= 1e21) {
      num /= 1e6;
      exp += 6;
    } else if (num >= 1e20) {
      num /= 1e5;
      exp += 5;
    } else if (num >= 1e19) {
      num /= 1e4;
      exp += 4;
    } else if (num >= 1e18) {
      num /= 1e3;
      exp += 3;
    } else if (num >= 1e17) {
      num /= 1e2;
      exp += 2;
    } else if (num >= 1e16) {
      num /= 10;
      exp += 1;
    } else if (num < 1e-5) {
      num *= 1e21;
      exp -= 21;
    } else if (num < 1e-4) {
      num *= 1e20;
      exp -= 20;
    } else if (num < 1e-3) {
      num *= 1e19;
      exp -= 19;
    } else if (num < 1e-2) {
      num *= 1e18;
      exp -= 18;
    } else if (num < 1e-1) {
      num *= 1e17;
      exp -= 17;
    } else if (num < 1e0) {
      num *= 1e16;
      exp -= 16;
    } else if (num < 1e1) {
      num *= 1e15;
      exp -= 15;
    } else if (num < 1e2) {
      num *= 1e14;
      exp -= 14;
    } else if (num < 1e3) {
      num *= 1e13;
      exp -= 13;
    } else if (num < 1e4) {
      num *= 1e12;
      exp -= 12;
    } else if (num < 1e5) {
      num *= 1e11;
      exp -= 11;
    } else if (num < 1e6) {
      num *= 1e10;
      exp -= 10;
    } else if (num < 1e7) {
      num *= 1e9;
      exp -= 9;
    } else if (num < 1e8) {
      num *= 1e8;
      exp -= 8;
    } else if (num < 1e9) {
      num *= 1e7;
      exp -= 7;
    } else if (num < 1e10) {
      num *= 1e6;
      exp -= 6;
    } else if (num < 1e11) {
      num *= 1e5;
      exp -= 5;
    } else if (num < 1e12) {
      num *= 1e4;
      exp -= 4;
    } else if (num < 1e13) {
      num *= 1e3;
      exp -= 3;
    } else if (num < 1e14) {
      num *= 1e2;
      exp -= 2;
    } else if (num < 1e15) {
      num *= 10;
      exp -= 1;
    }
  }
  unsigned long mantissa = num;
  if (precision < 15) {
    int rounder = 1;
    for (int i = precision; i < 15; ++i) {
      rounder *= 10;
    }
    int rem = mantissa % rounder;
    if (rem >= rounder / 2) mantissa += rounder - rem;
  }
  int pos = 0;
  for (int i = 0; i < spaces; ++i) {
    str->value[pos++] = ' ';
  }
  str->value[pos++] = neg ? '-' : ' ';
  unsigned long divisor = 1000000000000000ul;
  str->value[pos++] = '0' + mantissa / divisor;
  mantissa = mantissa % divisor;
  divisor /= 10;
  if (precision > 0) {
    str->value[pos++] = '.';
    for (PInteger i = 0; i < precision; ++i) {
      str->value[pos++] = '0' + mantissa / divisor;
      mantissa = mantissa % divisor;
      divisor /= 10;
    }
  }
  str->value[pos++] = 'E';
  neg = exp < 0;
  if (neg) exp = -exp;
  str->value[pos++] = neg ? '-' : '+';
  str->value[pos++] = '0' + exp / 100;
  exp %= 100;
  str->value[pos++] = '0' + exp / 10;
  exp %= 10;
  str->value[pos++] = '0' + exp;
  str->len = pos;
}