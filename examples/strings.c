/* Program: STRINGS */
#include "pascual.h"

void LITERALS() {
  WRITE_c(&OUTPUT, '\'');
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(7, "foo'bar"));
  WRITELN(&OUTPUT);
  WRITE_c(&OUTPUT, 'a');
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(2, "ab"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(2, "ab"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(3, "abc"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(3, "abc"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(4, "abcd"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_of('a'));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_of('a'));
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, 1);
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, 0);
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, 0);
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, 0);
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, 1);
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, 0);
  WRITELN(&OUTPUT);
}

void VARIABLES() {
  unsigned char C;
  PString S;
  PString T;
  C = 'a';
  WRITE_c(&OUTPUT, C);
  WRITELN(&OUTPUT);
  S = str_of('b');
  WRITE_s(&OUTPUT, S);
  WRITELN(&OUTPUT);
  S = str_make(2, "cd");
  WRITE_s(&OUTPUT, S);
  WRITELN(&OUTPUT);
  T = cat_cs('e', S);
  WRITE_s(&OUTPUT, T);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, S);
  WRITELN(&OUTPUT);
  S = str_make(3, "ABC");
  WRITE_c(&OUTPUT, S.chr[1]);
  WRITE_c(&OUTPUT, S.chr[2]);
  WRITE_c(&OUTPUT, S.chr[3]);
  WRITELN(&OUTPUT);
  WRITE_i(&OUTPUT, LENGTH(&S));
  WRITELN(&OUTPUT);
}

void MANIPULATION() {
  PString S;
  PString T;
  S = str_make(5, "12345");
  T = COPY(&S, 2, 3);
  WRITE_s(&OUTPUT, str_make(11, "Expect 234 "));
  WRITE_s(&OUTPUT, T);
  WRITELN(&OUTPUT);
  DELETE(&S, 1, 3);
  WRITE_s(&OUTPUT, str_make(10, "Expect 45 "));
  WRITE_s(&OUTPUT, S);
  WRITELN(&OUTPUT);
}

PString CONCAT(PString A, PString B) {
  PString RESULT;
  RESULT = cat_ss(A, B);
  return RESULT;
}

void APPEND(PString *A, PString B) {
  *A = cat_ss(*A, B);
}

void ARGUMENTS() {
  PString S;
  WRITE_s(&OUTPUT, CONCAT(str_of('a'), str_make(2, "cd")));
  WRITELN(&OUTPUT);
  S = str_make(3, "abc");
  APPEND(&S, str_make(3, "def"));
  WRITE_s(&OUTPUT, S);
  WRITELN(&OUTPUT);
}

void CONVERSION() {
  enum enum1 { ONE, TWO, THREE } E;
  PString S;
  const char* enumvalues1[] = { "ONE", "TWO", "THREE" };
  STR_e(ONE, enumvalues1, &S);
  WRITE_s(&OUTPUT, S);
  WRITELN(&OUTPUT);
  STR_i(123, &S);
  WRITE_s(&OUTPUT, S);
  WRITELN(&OUTPUT);
  STR_b(1, &S);
  WRITE_s(&OUTPUT, S);
  WRITELN(&OUTPUT);
}

void pascual_main() {
  LITERALS();
  VARIABLES();
  MANIPULATION();
  ARGUMENTS();
  CONVERSION();
}
