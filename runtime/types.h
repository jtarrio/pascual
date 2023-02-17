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

typedef struct {
  unsigned int bits[1];
} PSet1;
typedef struct {
  unsigned int bits[2];
} PSet2;
typedef struct {
  unsigned int bits[3];
} PSet3;
typedef struct {
  unsigned int bits[4];
} PSet4;
typedef struct {
  unsigned int bits[5];
} PSet5;
typedef struct {
  unsigned int bits[6];
} PSet6;
typedef struct {
  unsigned int bits[7];
} PSet7;
typedef struct {
  unsigned int bits[8];
} PSet8;

int subrange(int value, int low, int high);
int pred(int value, int low, int high);
int succ(int value, int low, int high);

#endif  // __PASCUAL_TYPES_H
