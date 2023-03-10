/* Program: SUBROUTINES */
#include "pascual.h"

void SAYHELLO() {
  WRITE_s(&OUTPUT, str_make(6, "Hello!"));
  WRITELN(&OUTPUT);
}

void SAY(PString WHAT, PBoolean EXCLAMATION) {
  WRITE_s(&OUTPUT, WHAT);
  if (EXCLAMATION) WRITE_c(&OUTPUT, '!');
  WRITELN(&OUTPUT);
}

void SWAP(PInteger *A, PInteger *B) {
  PInteger C;
  C = *A;
  *A = *B;
  *B = C;
}

void SAYCONSTREF(const PString *WHAT) {
  WRITE_s(&OUTPUT, *WHAT);
  WRITELN(&OUTPUT);
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
  WRITE_s(&OUTPUT, str_make(6, "First "));
  WRITE_i(&OUTPUT, LEVEL);
  WRITE_c(&OUTPUT, ' ');
  WRITE_s(&OUTPUT, NAME);
  WRITELN(&OUTPUT);
  if (LEVEL > 1) SECOND(NAME, LEVEL - 1);
}

void SECOND(PString NAME, PInteger LEVEL) {
  WRITE_s(&OUTPUT, str_make(7, "Second "));
  WRITE_i(&OUTPUT, LEVEL);
  WRITE_c(&OUTPUT, ' ');
  WRITE_s(&OUTPUT, NAME);
  WRITELN(&OUTPUT);
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
  WRITE_s(&OUTPUT, str_make(2, "x="));
  WRITE_i(&OUTPUT, X);
  WRITE_s(&OUTPUT, str_make(3, " y="));
  WRITE_i(&OUTPUT, Y);
  WRITELN(&OUTPUT);
  SWAP(&X, &Y);
  WRITE_s(&OUTPUT, str_make(2, "x="));
  WRITE_i(&OUTPUT, X);
  WRITE_s(&OUTPUT, str_make(3, " y="));
  WRITE_i(&OUTPUT, Y);
  WRITELN(&OUTPUT);
  {
    PString tmp1 = str_make(5, "Hello");
    SAYCONSTREF(&tmp1);
  }
  X = ADD(Y, 3);
  X = ADD(Y, MULTIPLY(X, 2));
  WRITE_i(&OUTPUT, FIB(10));
  WRITELN(&OUTPUT);
}
