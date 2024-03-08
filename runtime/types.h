#ifndef __PASCUAL_TYPES_H
#define __PASCUAL_TYPES_H

#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#define PNil ((void*)0)

typedef uint8_t PBits8;
typedef int8_t PBits8S;
typedef uint16_t PBits16;
typedef int16_t PBits16S;
typedef uint32_t PBits32;
typedef int32_t PBits32S;

typedef int POrdinal;

typedef int PBoolean;
typedef int PInteger;
typedef double PReal;
typedef unsigned char PChar;

typedef struct {
  union {
    PChar chr[256];
    struct __attribute__((__packed__)) {
      PChar len;
      PChar value[255];
    };
  };
} PString;

typedef struct {
  FILE* handle;
  PInteger block_size;
  PString name;
} PFile;

#define DEF_SET_BITS(b)                        \
  typedef struct __attribute__((__packed__)) { \
    unsigned char bits[b / 8];                 \
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

PInteger subrange(PInteger value, PInteger low, PInteger high);
PInteger pred(PInteger value, PInteger low, PInteger high);
PInteger succ(PInteger value, PInteger low, PInteger high);

#define New(ptr, size) GETMEM(ptr, size)
#define Dispose(ptr) FREEMEM(ptr, 0)
void GETMEM(void** ptr, PInteger size);
void FREEMEM(void** ptr, PInteger size);

#endif  // __PASCUAL_TYPES_H
