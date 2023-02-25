#include "sets.h"

void set_union(const unsigned char* a, const unsigned char* b,
               unsigned char* dst, int bytes) {
  for (int i = 0; i < bytes; ++i) {
    dst[i] = a[i] | b[i];
  }
}

void set_difference(const unsigned char* a, const unsigned char* b,
                    unsigned char* dst, int bytes) {
  for (int i = 0; i < bytes; ++i) {
    dst[i] = a[i] & ~b[i];
  }
}

void set_intersection(const unsigned char* a, const unsigned char* b,
                      unsigned char* dst, int bytes) {
  for (int i = 0; i < bytes; ++i) {
    dst[i] = a[i] & b[i];
  }
}

int set_equals(const unsigned char* a, const unsigned char* b, int bytes) {
  int equals = 1;
  for (int i = 0; i < bytes && equals; ++i) {
    equals = equals && a[i] == b[i];
  }
  return equals;
}

int set_issuperset(const unsigned char* big, const unsigned char* small,
                 int bytes) {
  int issuperset = 1;
  for (int i = 0; i < bytes; ++i) {
    issuperset = issuperset && (small[i] & ~big[i]) == 0;
  }
  return issuperset;
}

int set_in(int ordinal, int lowboundbyte, const unsigned char* bits) {
  int bytenum = (ordinal / 8) - lowboundbyte;
  int bitnum = ordinal % 8;
  return (bits[bytenum] & (1 << bitnum)) != 0;
}

void set_set(int first, int last, int lowboundbyte, unsigned char* bits) {
  if (first > last) return;
  for (int i = first; i <= last; ++i) {
    int bytenum = (i / 8) - lowboundbyte;
    int bitnum = i % 8;
    bits[bytenum] = bits[bytenum] | (1 << bitnum);
  }
}
