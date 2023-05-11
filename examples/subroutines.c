/* Program: SUBROUTINES */
#include "pascual.h"

PInteger X;
PInteger Y;

void SECOND(PString NAME, PInteger LEVEL);

void SAYHELLO() {
  Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 6, "Hello!");
}

void SAY(PString WHAT, PBoolean EXCLAMATION) {
  Write(&OUTPUT, 1, RwpStringPtr | RwpEnd, &WHAT);
  if (EXCLAMATION) Write(&OUTPUT, 1, RwpChar | RwpEnd, '!');
  Write(&OUTPUT, 1, RwpEnd | RwpLn);
}

void SWAP(PInteger* A, PInteger* B) {
  PInteger C;
  C = *A;
  *A = *B;
  *B = C;
}

void SAYCONSTREF(const PString* WHAT) {
  Write(&OUTPUT, 1, RwpStringPtr | RwpLn | RwpEnd, WHAT);
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

void FIRST(PString NAME, PInteger LEVEL) {
  Write(&OUTPUT, 1, RwpLenPtr, 6, "First ", RwpInt, LEVEL, RwpChar, ' ', RwpStringPtr | RwpLn | RwpEnd, &NAME);
  if (LEVEL > 1) SECOND(NAME, LEVEL - 1);
}

void SECOND(PString NAME, PInteger LEVEL) {
  Write(&OUTPUT, 1, RwpLenPtr, 7, "Second ", RwpInt, LEVEL, RwpChar, ' ', RwpStringPtr | RwpLn | RwpEnd, &NAME);
  if (LEVEL > 1) FIRST(NAME, LEVEL - 1);
}

void pascual_main() {
  PString tmp1;
  SAYHELLO();
  SAYHELLO();
  SAY(str_make(4, "Hola"), 1);
  X = 1;
  Y = 2;
  Write(&OUTPUT, 1, RwpLenPtr, 2, "x=", RwpInt, X, RwpLenPtr, 3, " y=", RwpInt | RwpLn | RwpEnd, Y);
  SWAP(&X, &Y);
  Write(&OUTPUT, 1, RwpLenPtr, 2, "x=", RwpInt, X, RwpLenPtr, 3, " y=", RwpInt | RwpLn | RwpEnd, Y);
  tmp1 = str_make(5, "Hello");
  SAYCONSTREF(&tmp1);
  X = ADD(Y, 3);
  X = ADD(Y, MULTIPLY(X, 2));
  Write(&OUTPUT, 1, RwpInt | RwpLn | RwpEnd, FIB(10));
}
