/* Program: TYPES */
#include "pascual.h"

typedef int BOO;
typedef enum enum1 { ONE, TWO, THREE } NUMBERS;
typedef NUMBERS NUMS;
typedef struct record1 {
  int NUM;
  PString STR;
} REC;
typedef struct record2 {
  int BOL;
  REC REC;
} REC2;
typedef int ARR1[10];
typedef REC ARR2[5];

const char* enumvalues1[] = { "ONE", "TWO", "THREE" };

BOO SCARED;
NUMBERS ANUMBER;
NUMS ANUM;
REC AREC;
REC2 AREC2;
enum enum2 { LETTERA, LETTERB, LETTERC, LETTERD } ALETTER;
struct record3 {
  PString FIRST;
  PString LAST;
} ANAME;
ARR1 AARR1;
PString AARR3[20];

const char* enumvalues2[] = { "LETTERA", "LETTERB", "LETTERC", "LETTERD" };

void PROC() {
  enum enum3 { UN, DEUX, TROIS } FR;
  const char* enumvalues3[] = { "UN", "DEUX", "TROIS" };
  WRITE_e(&OUTPUT, DEUX, enumvalues3);
  WRITELN(&OUTPUT);
}

int FUN() {
  int RESULT;
  enum enum3 { EINS, ZWEI, DREI } DE;
  const char* enumvalues3[] = { "EINS", "ZWEI", "DREI" };
  WRITE_e(&OUTPUT, DREI, enumvalues3);
  WRITELN(&OUTPUT);
  RESULT = 0;
  return RESULT;
}

void ARRAYS() {
  int POS;
  ARR2 BARR2;
  int BARR4[10];
  do {
    int first = 1;
    int last = 10;
    if (first <= last) {
      POS = first;
      while (1) {
        AARR1[(int)subrange(POS, 1, 10) - 1] = POS;
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  do {
    int first = 1;
    int last = 10;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          STR_i(AARR1[(int)subrange(POS, 1, 10) - 1], &AARR3[(int)subrange(POS, 1, 20) - 1]);
          STR_i(10 + AARR1[(int)subrange(POS, 1, 10) - 1] * 2, &AARR3[(int)subrange(POS + 10, 1, 20) - 1]);
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  BARR2[0].STR = str_make(11, "In an array");
  do {
    int first = 1;
    int last = 10;
    if (first <= last) {
      POS = first;
      while (1) {
        BARR4[(int)subrange(POS, 1, 10) - 1] = AARR1[(int)subrange(POS, 1, 10) - 1] % 2 == 0;
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  do {
    int first = 1;
    int last = 10;
    if (first <= last) {
      POS = first;
      while (1) {
        WRITE_s(&OUTPUT, AARR3[(int)subrange(POS, 1, 20) - 1]);
        WRITE_c(&OUTPUT, ' ');
        WRITE_i(&OUTPUT, AARR1[(int)subrange(POS, 1, 10) - 1]);
        WRITE_c(&OUTPUT, ' ');
        WRITE_b(&OUTPUT, BARR4[(int)subrange(POS, 1, 10) - 1]);
        WRITELN(&OUTPUT);
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  do {
    int first = 11;
    int last = 20;
    if (first <= last) {
      POS = first;
      while (1) {
        WRITE_s(&OUTPUT, AARR3[(int)subrange(POS, 1, 20) - 1]);
        WRITELN(&OUTPUT);
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  WRITE_s(&OUTPUT, BARR2[0].STR);
  WRITELN(&OUTPUT);
}

void pascual_main() {
  SCARED = 1;
  ANUMBER = TWO;
  ANUM = THREE;
  AREC.NUM = 123;
  AREC.STR = str_make(13, "four five six");
  AREC2.BOL = 1;
  AREC2.REC.NUM = 456;
  AREC2.REC.STR = str_make(16, "seven eight nine");
  ALETTER = LETTERD;
  ANAME.FIRST = str_make(8, "Sherlock");
  ANAME.LAST = str_make(6, "Holmes");
  WRITE_b(&OUTPUT, SCARED);
  WRITELN(&OUTPUT);
  WRITE_e(&OUTPUT, ANUMBER, enumvalues1);
  WRITELN(&OUTPUT);
  WRITE_e(&OUTPUT, ANUM, enumvalues1);
  WRITELN(&OUTPUT);
  WRITE_i(&OUTPUT, AREC.NUM);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, AREC.STR);
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, AREC2.BOL);
  WRITELN(&OUTPUT);
  WRITE_i(&OUTPUT, AREC2.REC.NUM);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, AREC2.REC.STR);
  WRITELN(&OUTPUT);
  WRITE_e(&OUTPUT, ALETTER, enumvalues2);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, ANAME.FIRST);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, ANAME.LAST);
  WRITELN(&OUTPUT);
  PROC();
  WRITE_i(&OUTPUT, FUN());
  WRITELN(&OUTPUT);
  ARRAYS();
}
