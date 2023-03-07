#ifndef __PASCUAL_SETS_H
#define __PASCUAL_SETS_H

#include "types.h"

void set_union(const unsigned char* a, const unsigned char* b,
               unsigned char* dst, int bytes);
void set_difference(const unsigned char* a, const unsigned char* b,
                    unsigned char* dst, int bytes);
void set_intersection(const unsigned char* a, const unsigned char* b,
                      unsigned char* dst, int bytes);
PBoolean set_equals(const unsigned char* a, const unsigned char* b, int bytes);
PBoolean set_issuperset(const unsigned char* big, const unsigned char* small,
                 int bytes);
PBoolean set_in(POrdinal ordinal, int lowboundbyte, const unsigned char* bits);
void set_set(POrdinal first, POrdinal last, int lowboundbyte, unsigned char* bits);

#endif  // __PASCUAL_SETS_H