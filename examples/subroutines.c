/* Program: SUBROUTINES */
#include "pascual.h"

void SAYHELLO() {
  WRITE_s(&OUTPUT, str_make(6, "Hello!"));
  WRITELN(&OUTPUT);
}

void SAY(PString WHAT, int EXCLAMATION) {
  WRITE_s(&OUTPUT, WHAT);
  if (EXCLAMATION) WRITE_c(&OUTPUT, '!');
  WRITELN(&OUTPUT);
}

void SWAP(int *A, int *B) {
  int C;
  C = *A;
  *A = *B;
  *B = C;
}

void SAYCONSTREF(const PString *WHAT) {
  WRITE_s(&OUTPUT, *WHAT);
  WRITELN(&OUTPUT);
}

int ADD(int A, int B) {
  int RESULT;
  RESULT = A + B;
  return RESULT;
}

int MULTIPLY(int A, int B) {
  int RESULT;
  RESULT = A;
  RESULT = RESULT * B;
  return RESULT;
}

int FIB(int N) {
  int RESULT;
  if (N <= 1) RESULT = 1;
  else RESULT = FIB(N - 1) + FIB(N - 2);
  return RESULT;
}

void SECOND(PString NAME, int LEVEL);

void FIRST(PString NAME, int LEVEL) {
  WRITE_s(&OUTPUT, str_make(6, "First "));
  WRITE_i(&OUTPUT, LEVEL);
  WRITE_c(&OUTPUT, ' ');
  WRITE_s(&OUTPUT, NAME);
  WRITELN(&OUTPUT);
  if (LEVEL > 1) SECOND(NAME, LEVEL - 1);
}

void SECOND(PString NAME, int LEVEL) {
  WRITE_s(&OUTPUT, str_make(7, "Second "));
  WRITE_i(&OUTPUT, LEVEL);
  WRITE_c(&OUTPUT, ' ');
  WRITE_s(&OUTPUT, NAME);
  WRITELN(&OUTPUT);
  if (LEVEL > 1) FIRST(NAME, LEVEL - 1);
}

int X;
int Y;

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
