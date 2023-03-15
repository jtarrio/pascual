/* Program: SUBROUTINES */
#include "pascual.h"

void SAYHELLO() {
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 6, "Hello!");
}

void SAY(PString WHAT, PBoolean EXCLAMATION) {
  WRITE(&OUTPUT, RwpStringPtr | RwpEnd, &WHAT);
  if (EXCLAMATION) WRITE(&OUTPUT, RwpChar | RwpEnd, '!');
  WRITE(&OUTPUT, RwpEnd | RwpLn);
}

void SWAP(PInteger *A, PInteger *B) {
  PInteger C;
  C = *A;
  *A = *B;
  *B = C;
}

void SAYCONSTREF(const PString *WHAT) {
  WRITE(&OUTPUT, RwpStringPtr | RwpLn | RwpEnd, WHAT);
}

PInteger ADD(PInteger A, PInteger B) {
  PInteger RESULT;
  RESULT = A + B;
  return RESULT;
}

PInteger MULTIPLY(PInteger A, PInteger B) {
  PInteger RESULT;
  RESULT = A;
  RESULT = RESULT * B;
  return RESULT;
}

PInteger FIB(PInteger N) {
  PInteger RESULT;
  if (N <= 1) RESULT = 1;
  else RESULT = FIB(N - 1) + FIB(N - 2);
  return RESULT;
}

void SECOND(PString NAME, PInteger LEVEL);

void FIRST(PString NAME, PInteger LEVEL) {
  WRITE(&OUTPUT, RwpLenPtr, 6, "First ", RwpInt, LEVEL, RwpChar, ' ', RwpStringPtr | RwpLn | RwpEnd, &NAME);
  if (LEVEL > 1) SECOND(NAME, LEVEL - 1);
}

void SECOND(PString NAME, PInteger LEVEL) {
  WRITE(&OUTPUT, RwpLenPtr, 7, "Second ", RwpInt, LEVEL, RwpChar, ' ', RwpStringPtr | RwpLn | RwpEnd, &NAME);
  if (LEVEL > 1) FIRST(NAME, LEVEL - 1);
}

PInteger X;
PInteger Y;

void pascual_main() {
  SAYHELLO();
  SAYHELLO();
  SAY(str_make(4, "Hola"), 1);
  X = 1;
  Y = 2;
  WRITE(&OUTPUT, RwpLenPtr, 2, "x=", RwpInt, X, RwpLenPtr, 3, " y=", RwpInt | RwpLn | RwpEnd, Y);
  SWAP(&X, &Y);
  WRITE(&OUTPUT, RwpLenPtr, 2, "x=", RwpInt, X, RwpLenPtr, 3, " y=", RwpInt | RwpLn | RwpEnd, Y);
  {
    PString tmp1 = str_make(5, "Hello");
    SAYCONSTREF(&tmp1);
  }
  X = ADD(Y, 3);
  X = ADD(Y, MULTIPLY(X, 2));
  WRITE(&OUTPUT, RwpInt | RwpLn | RwpEnd, FIB(10));
}
