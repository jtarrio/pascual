/* Program: PROCEDURES */
#include "pascual.h"

int NUM;

void SAYHELLO() {
  WRITE_s(&OUTPUT, str_make(6, "Hello!"));
  WRITELN(&OUTPUT);
}

void DISPLAY(int NUM);

void DISPLAY(int NUM) {
  WRITE_i(&OUTPUT, NUM);
  WRITELN(&OUTPUT);
}

void SUM1(int *NUM) {
  *NUM = *NUM + 1;
}

int SUM(int N1, int N2) {
  int RESULT;
  RESULT = N1 + N2;
  return RESULT;
}

int SUMINPLACE(int *N1, int N2) {
  int RESULT;
  *N1 = *N1 + N2;
  RESULT = *N1;
  return RESULT;
}

int FIBONACCI(int N) {
  int RESULT;
  if (N <= 1) RESULT = 1;
  else RESULT = FIBONACCI(N - 1) + FIBONACCI(N - 2);
  return RESULT;
}

int THREE() {
  int RESULT;
  RESULT = 3;
  return RESULT;
}

void pascual_main() {
  SAYHELLO();
  WRITELN(&OUTPUT);
  DISPLAY(THREE());
  NUM = 1;
  DISPLAY(NUM);
  SUM1(&NUM);
  DISPLAY(NUM);
  DISPLAY(SUM(NUM, 1));
  DISPLAY(SUMINPLACE(&NUM, 2));
  DISPLAY(NUM);
  do {
    int first = 1;
    int last = 5;
    if (first <= last) {
      NUM = first;
      while (1) {
        DISPLAY(FIBONACCI(NUM));
        if (NUM == last) break;
        ++NUM;
      }
    }
  } while(0);
}
