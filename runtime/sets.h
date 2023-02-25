#ifndef __PASCUAL_SETS_H
#define __PASCUAL_SETS_H

void set_union(const unsigned char* a, const unsigned char* b,
               unsigned char* dst, int bytes);
void set_difference(const unsigned char* a, const unsigned char* b,
                    unsigned char* dst, int bytes);
void set_intersection(const unsigned char* a, const unsigned char* b,
                      unsigned char* dst, int bytes);
int set_equals(const unsigned char* a, const unsigned char* b, int bytes);
int set_issuperset(const unsigned char* big, const unsigned char* small,
                 int bytes);
int set_in(int ordinal, int lowboundbyte, const unsigned char* bits);
void set_set(int first, int last, int lowboundbyte, unsigned char* bits);

#endif  // __PASCUAL_SETS_H