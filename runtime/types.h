#ifndef __PASCUAL_TYPES_H
#define __PASCUAL_TYPES_H

#include <limits.h>
#include <stdio.h>

typedef struct {
  union {
    unsigned char chr[256];
    struct __attribute__((__packed__)) {
      unsigned char len;
      unsigned char value[255];
    };
  };
} PString;

typedef struct {
  PString name;
  FILE* file;
} PFile;

#define DEF_SET_BITS(b)                        \
  typedef struct __attribute__((__packed__)) { \
    unsigned int bits[b / 8];                  \
  } PSet##b
DEF_SET_BITS(8);
DEF_SET_BITS(16);
DEF_SET_BITS(24);
DEF_SET_BITS(32);
DEF_SET_BITS(40);
DEF_SET_BITS(48);
DEF_SET_BITS(56);
DEF_SET_BITS(64);
DEF_SET_BITS(72);
DEF_SET_BITS(80);
DEF_SET_BITS(88);
DEF_SET_BITS(96);
DEF_SET_BITS(104);
DEF_SET_BITS(112);
DEF_SET_BITS(120);
DEF_SET_BITS(128);
DEF_SET_BITS(136);
DEF_SET_BITS(144);
DEF_SET_BITS(152);
DEF_SET_BITS(160);
DEF_SET_BITS(168);
DEF_SET_BITS(176);
DEF_SET_BITS(184);
DEF_SET_BITS(192);
DEF_SET_BITS(200);
DEF_SET_BITS(208);
DEF_SET_BITS(216);
DEF_SET_BITS(224);
DEF_SET_BITS(232);
DEF_SET_BITS(240);
DEF_SET_BITS(248);
DEF_SET_BITS(256);
#undef DEF_SET_BITS

int subrange(int value, int low, int high);
int pred(int value, int low, int high);
int succ(int value, int low, int high);

#endif  // __PASCUAL_TYPES_H
