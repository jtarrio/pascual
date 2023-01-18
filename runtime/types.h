#ifndef __PASCUAL_TYPES_H
#define __PASCUAL_TYPES_H

#include <stdio.h>

typedef int PBoolean;

typedef struct {
  union {
    char chr[256];
    struct __attribute__((__packed__)) {
      unsigned char len;
      char value[255];
    };
  };
} PString;

typedef struct {
  PString name;
  FILE* file;
} PFile;

#endif  // __PASCUAL_TYPES_H
