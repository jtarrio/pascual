/* Program: FLOWCONTROL */
#include "pascual.h"

void TESTIF() {
  WRITE_s(&OUTPUT, str_make(2, "if"));
  WRITELN(&OUTPUT);
  if (1) {
    WRITE_s(&OUTPUT, str_make(4, "1 ok"));
    WRITELN(&OUTPUT);
  }
  if (0) {
    WRITE_s(&OUTPUT, str_make(4, "1 no"));
    WRITELN(&OUTPUT);
  }
  if (1) {
    WRITE_s(&OUTPUT, str_make(4, "2 ok"));
    WRITELN(&OUTPUT);
  }
  else {
    WRITE_s(&OUTPUT, str_make(4, "2 no"));
    WRITELN(&OUTPUT);
  }
  if (0) {
    WRITE_s(&OUTPUT, str_make(4, "3 no"));
    WRITELN(&OUTPUT);
  }
  else {
    WRITE_s(&OUTPUT, str_make(4, "3 ok"));
    WRITELN(&OUTPUT);
  }
  if (1) if (1) {
    WRITE_s(&OUTPUT, str_make(4, "4 ok"));
    WRITELN(&OUTPUT);
  }
  else {
    WRITE_s(&OUTPUT, str_make(4, "4 no"));
    WRITELN(&OUTPUT);
  }
  if (1) if (0) {
    WRITE_s(&OUTPUT, str_make(4, "5 no"));
    WRITELN(&OUTPUT);
  }
  else {
    WRITE_s(&OUTPUT, str_make(4, "5 ok"));
    WRITELN(&OUTPUT);
  }
  if (1) ;
  if (1) ;
  else {
    WRITE_s(&OUTPUT, str_make(4, "6 no"));
    WRITELN(&OUTPUT);
  }
  if (0) ;
  else {
    WRITE_s(&OUTPUT, str_make(4, "6 ok"));
    WRITELN(&OUTPUT);
  }
}

void TESTREPEAT() {
  int I;
  WRITE_s(&OUTPUT, str_make(6, "repeat"));
  WRITELN(&OUTPUT);
  I = 1;
  do {
    WRITE_s(&OUTPUT, str_make(2, "1 "));
    WRITE_i(&OUTPUT, I);
    WRITELN(&OUTPUT);
    I = I + 1;
  } while (!(I > 5));
  WRITE_s(&OUTPUT, str_make(12, "1 expect 6: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  I = 5;
  do {
    WRITE_s(&OUTPUT, str_make(2, "2 "));
    WRITE_i(&OUTPUT, I);
    WRITELN(&OUTPUT);
    I = I + 1;
  } while (!(I > 5));
  WRITE_s(&OUTPUT, str_make(12, "2 expect 6: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  I = 6;
  do {
    WRITE_s(&OUTPUT, str_make(2, "3 "));
    WRITE_i(&OUTPUT, I);
    WRITELN(&OUTPUT);
    I = I + 1;
  } while (!(I > 5));
  WRITE_s(&OUTPUT, str_make(12, "3 expect 7: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  I = 7;
  do {
    WRITE_s(&OUTPUT, str_make(2, "4 "));
    WRITE_i(&OUTPUT, I);
    WRITELN(&OUTPUT);
    I = I + 1;
  } while (!(I > 5));
  WRITE_s(&OUTPUT, str_make(12, "3 expect 8: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  do {
  } while (0);
  do {
    ;
  } while (0);
}

void TESTWHILE() {
  int I;
  WRITE_s(&OUTPUT, str_make(5, "while"));
  WRITELN(&OUTPUT);
  I = 1;
  while (I <= 5) {
    WRITE_s(&OUTPUT, str_make(2, "1 "));
    WRITE_i(&OUTPUT, I);
    WRITELN(&OUTPUT);
    I = I + 1;
  }
  WRITE_s(&OUTPUT, str_make(12, "1 expect 6: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  I = 5;
  while (I <= 5) {
    WRITE_s(&OUTPUT, str_make(2, "2 "));
    WRITE_i(&OUTPUT, I);
    WRITELN(&OUTPUT);
    I = I + 1;
  }
  WRITE_s(&OUTPUT, str_make(12, "2 expect 6: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  I = 6;
  while (I <= 5) {
    WRITE_s(&OUTPUT, str_make(2, "3 "));
    WRITE_i(&OUTPUT, I);
    WRITE_s(&OUTPUT, str_make(3, " no"));
    WRITELN(&OUTPUT);
    I = I + 1;
  }
  WRITE_s(&OUTPUT, str_make(12, "3 expect 6: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  I = 7;
  while (I <= 5) {
    WRITE_s(&OUTPUT, str_make(2, "4 "));
    WRITE_i(&OUTPUT, I);
    WRITE_s(&OUTPUT, str_make(3, " no"));
    WRITELN(&OUTPUT);
    I = I + 1;
  }
  WRITE_s(&OUTPUT, str_make(12, "4 expect 7: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  while (0) ;
}

void TESTFOR() {
  int I;
  WRITE_s(&OUTPUT, str_make(3, "for"));
  WRITELN(&OUTPUT);
  do {
    int first = 1;
    int last = 5;
    if (first <= last) {
      I = first;
      while (1) {
        WRITE_s(&OUTPUT, str_make(2, "1 "));
        WRITE_i(&OUTPUT, I);
        WRITELN(&OUTPUT);
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
  WRITE_s(&OUTPUT, str_make(12, "1 expect 5: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  do {
    int first = 6;
    int last = 5;
    if (first <= last) {
      I = first;
      while (1) {
        WRITE_s(&OUTPUT, str_make(2, "2 "));
        WRITE_i(&OUTPUT, I);
        WRITE_s(&OUTPUT, str_make(3, " no"));
        WRITELN(&OUTPUT);
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
  WRITE_s(&OUTPUT, str_make(12, "2 expect 5: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  do {
    int first = 5;
    int last = 1;
    if (first >= last) {
      I = first;
      while (1) {
        WRITE_s(&OUTPUT, str_make(2, "3 "));
        WRITE_i(&OUTPUT, I);
        WRITELN(&OUTPUT);
        if (I == last) break;
        --I;
      }
    }
  } while(0);
  WRITE_s(&OUTPUT, str_make(12, "3 expect 1: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  do {
    int first = 0;
    int last = 1;
    if (first >= last) {
      I = first;
      while (1) {
        WRITE_s(&OUTPUT, str_make(2, "4 "));
        WRITE_i(&OUTPUT, I);
        WRITE_s(&OUTPUT, str_make(3, " no"));
        WRITELN(&OUTPUT);
        if (I == last) break;
        --I;
      }
    }
  } while(0);
  WRITE_s(&OUTPUT, str_make(12, "4 expect 1: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
  do {
    int first = 1;
    int last = 5;
    if (first <= last) {
      I = first;
      while (1) {
        ;
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
}

void pascual_main() {
  TESTIF();
  TESTREPEAT();
  TESTWHILE();
  TESTFOR();
}
