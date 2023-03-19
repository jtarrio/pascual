/* Program: POINTERS */
#include "pascual.h"

typedef PString* STRINGP;
typedef struct record1 {
  PInteger A;
  PBoolean B;
} MYRECORD;
typedef MYRECORD* MYRECORDP;
typedef struct record2* NODEP;
typedef struct record2 {
  PInteger VALUE;
  struct record2* PREV;
  struct record2* NEXT;
} NODE;

PString* MYSTRPTR;
MYRECORD* MYRECPTR;
NODE* ROOTNODE;
PInteger* MYINTPTR;
PInteger MYINT;

void ADDNODE(PInteger VALUE) {
  NODE* PREVROOT;
  PREVROOT = ROOTNODE;
  ROOTNODE = malloc(sizeof(NODE));
  ROOTNODE->VALUE = VALUE;
  ROOTNODE->NEXT = PREVROOT;
  if (PREVROOT == PNil) ROOTNODE->PREV = PNil;
  else {
    ROOTNODE->PREV = PREVROOT->PREV;
    PREVROOT->PREV = ROOTNODE;
  }
}

NODE* FINDNODE(PInteger COUNT) {
  NODE* RESULT;
  NODE* NODE;
  NODE = ROOTNODE;
  while (COUNT > 1 && NODE != PNil) {
    if (NODE != PNil) NODE = NODE->NEXT;
    COUNT = COUNT - 1;
  }
  RESULT = NODE;
  return RESULT;
}

void DISPOSELIST() {
  NODE* NEXT;
  while (ROOTNODE != PNil) {
    NEXT = ROOTNODE->NEXT;
    free(ROOTNODE);
    ROOTNODE = NEXT;
  }
}

void pascual_main() {
  MYSTRPTR = malloc(sizeof(PString));
  *MYSTRPTR = str_make(3, "foo");
  WRITE(&OUTPUT, RwpStringPtr | RwpLn | RwpEnd, MYSTRPTR);
  free(MYSTRPTR);
  MYRECPTR = malloc(sizeof(MYRECORD));
  MYRECPTR->A = 123;
  MYRECPTR->B = 1;
  WRITE(&OUTPUT, RwpInt, MYRECPTR->A, RwpChar, ' ', RwpBool | RwpLn | RwpEnd, MYRECPTR->B);
  free(MYRECPTR);
  MYINTPTR = malloc(sizeof(PInteger));
  *MYINTPTR = 42;
  WRITE(&OUTPUT, RwpInt | RwpLn | RwpEnd, *MYINTPTR);
  free(MYINTPTR);
  MYINT = 123456;
  MYINTPTR = &MYINT;
  WRITE(&OUTPUT, RwpInt | RwpLn | RwpEnd, *MYINTPTR);
  ROOTNODE = PNil;
  ADDNODE(123);
  ADDNODE(456);
  ADDNODE(789);
  WRITE(&OUTPUT, RwpInt | RwpLn | RwpEnd, FINDNODE(2)->VALUE);
  DISPOSELIST();
}
