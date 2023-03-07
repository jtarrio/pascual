/* Program: POINTERS */
#include "pascual.h"

typedef PString *STRINGP;
typedef struct record1 {
  PInteger A;
  PBoolean B;
} MYRECORD;
typedef MYRECORD *MYRECORDP;
typedef struct record2 *NODEP;
typedef struct record2 {
  PInteger VALUE;
  struct record2 *PREV;
  struct record2 *NEXT;
} NODE;

PString *MYSTR;
MYRECORD *MYREC;
NODE *ROOTNODE;
PInteger *MYINT;

void ADDNODE(PInteger VALUE) {
  NODE *PREVROOT;
  PREVROOT = ROOTNODE;
  ROOTNODE = malloc(sizeof(NODE));
  ROOTNODE->VALUE = VALUE;
  ROOTNODE->NEXT = PREVROOT;
  if (PREVROOT == (void*)0) ROOTNODE->PREV = (void*)0;
  else {
    ROOTNODE->PREV = PREVROOT->PREV;
    PREVROOT->PREV = ROOTNODE;
  }
}

NODE *FINDNODE(PInteger COUNT) {
  NODE *RESULT;
  NODE *NODE;
  NODE = ROOTNODE;
  while (COUNT > 1 && NODE != (void*)0) {
    if (NODE != (void*)0) NODE = NODE->NEXT;
    COUNT = COUNT - 1;
  }
  RESULT = NODE;
  return RESULT;
}

void DISPOSELIST() {
  NODE *NEXT;
  while (ROOTNODE != (void*)0) {
    NEXT = ROOTNODE->NEXT;
    free(ROOTNODE);
    ROOTNODE = NEXT;
  }
}

void pascual_main() {
  MYSTR = malloc(sizeof(PString));
  *MYSTR = str_make(3, "foo");
  WRITE_s(&OUTPUT, *MYSTR);
  WRITELN(&OUTPUT);
  free(MYSTR);
  MYREC = malloc(sizeof(MYRECORD));
  MYREC->A = 123;
  MYREC->B = 1;
  WRITE_i(&OUTPUT, MYREC->A);
  WRITE_c(&OUTPUT, ' ');
  WRITE_b(&OUTPUT, MYREC->B);
  WRITELN(&OUTPUT);
  free(MYREC);
  MYINT = malloc(sizeof(PInteger));
  *MYINT = 42;
  WRITE_i(&OUTPUT, *MYINT);
  WRITELN(&OUTPUT);
  free(MYINT);
  ROOTNODE = (void*)0;
  ADDNODE(123);
  ADDNODE(456);
  ADDNODE(789);
  WRITE_i(&OUTPUT, FINDNODE(2)->VALUE);
  WRITELN(&OUTPUT);
  DISPOSELIST();
}
