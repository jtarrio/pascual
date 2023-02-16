/* Program: FLOWCONTROL */
#include "pascual.h"

void SHOWIF() {
  if (1) {
    WRITE_s(&OUTPUT, str_make(4, "true"));
    WRITELN(&OUTPUT);
  }
  else {
    WRITE_s(&OUTPUT, str_make(5, "false"));
    WRITELN(&OUTPUT);
  }
  if (1) {
    WRITE_s(&OUTPUT, str_make(4, "true"));
    WRITELN(&OUTPUT);
  }
  if (1) ;
  else {
    WRITE_s(&OUTPUT, str_make(5, "false"));
    WRITELN(&OUTPUT);
  }
  if (1) ;
  if (1) {
    WRITE_s(&OUTPUT, str_make(4, "true"));
    WRITELN(&OUTPUT);
    WRITE_s(&OUTPUT, str_make(8, "true dat"));
    WRITELN(&OUTPUT);
  }
  else {
    WRITE_s(&OUTPUT, str_make(5, "false"));
    WRITELN(&OUTPUT);
    WRITE_s(&OUTPUT, str_make(9, "no really"));
    WRITELN(&OUTPUT);
  }
  if (1) {
    WRITE_s(&OUTPUT, str_make(4, "true"));
    WRITELN(&OUTPUT);
  }
  else if (1) {
    WRITE_s(&OUTPUT, str_make(5, "true2"));
    WRITELN(&OUTPUT);
  }
  else {
    WRITE_s(&OUTPUT, str_make(5, "false"));
    WRITELN(&OUTPUT);
  }
  if (1) {
    if (1) {
      WRITE_s(&OUTPUT, str_make(4, "true"));
      WRITELN(&OUTPUT);
    }
    else {
      WRITE_s(&OUTPUT, str_make(5, "false"));
      WRITELN(&OUTPUT);
    }
  }
  if (1) if (1) {
    WRITE_s(&OUTPUT, str_make(4, "true"));
    WRITELN(&OUTPUT);
  }
  else {
    WRITE_s(&OUTPUT, str_make(5, "false"));
    WRITELN(&OUTPUT);
  }
}

void SHOWREPEATUNTIL() {
  int I;
  I = 0;
  do {
    I = I + 1;
  } while (!(I == 5));
  I = 0;
  do {
    I = I + 1;
    WRITE_i(&OUTPUT, I);
    WRITELN(&OUTPUT);
  } while (!(I == 5));
  I = 10;
  do {
    I = I + 1;
  } while (!(I == 5));
  do {
  } while (1);
}

void SHOWWHILE() {
  int I;
  I = 0;
  while (I != 5) I = I + 1;
  I = 0;
  while (I != 5) {
    I = I + 1;
    WRITE_i(&OUTPUT, I);
    WRITELN(&OUTPUT);
  }
  while (0) ;
}

void SHOWFOR() {
  int I;
  do {
    int first = 1;
    int last = 10;
    if (first <= last) {
      I = first;
      while (1) {
        WRITE_i(&OUTPUT, I);
        WRITELN(&OUTPUT);
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
  do {
    int first = 10;
    int last = 1;
    if (first >= last) {
      I = first;
      while (1) {
        WRITE_i(&OUTPUT, I);
        WRITELN(&OUTPUT);
        if (I == last) break;
        --I;
      }
    }
  } while(0);
  do {
    int first = 1;
    int last = 10;
    if (first <= last) {
      I = first;
      while (1) {
        {
          WRITE_s(&OUTPUT, str_make(14, "The number is "));
          WRITE_i(&OUTPUT, I);
          WRITELN(&OUTPUT);
        }
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
  I = 10;
  do {
    int first = 1;
    int last = I;
    if (first <= last) {
      I = first;
      while (1) {
        WRITE_i(&OUTPUT, I);
        WRITELN(&OUTPUT);
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
  I = 0;
  do {
    int first = 20;
    int last = 10;
    if (first <= last) {
      I = first;
      while (1) {
        WRITE_i(&OUTPUT, I);
        WRITELN(&OUTPUT);
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
  WRITE_s(&OUTPUT, str_make(29, "The value of i is 0, not 20: "));
  WRITE_i(&OUTPUT, I);
  WRITELN(&OUTPUT);
}

void SHOWCASE() {
  unsigned char I;
  I = 'b';
  switch (I) {
    case 'a':
      WRITE_c(&OUTPUT, 'a');
      WRITELN(&OUTPUT);
      break;
    case 'b':
      WRITE_c(&OUTPUT, 'b');
      WRITELN(&OUTPUT);
      break;
    case 'c':
      WRITE_c(&OUTPUT, 'c');
      WRITELN(&OUTPUT);
      break;
    default:
      break;
  }
  switch (I) {
    case 'd':
      WRITE_c(&OUTPUT, 'd');
      WRITELN(&OUTPUT);
      break;
    case 'e':
      WRITE_c(&OUTPUT, 'e');
      WRITELN(&OUTPUT);
      break;
    default:
      WRITE_s(&OUTPUT, str_make(5, "other"));
      WRITELN(&OUTPUT);
      break;
  }
}

void pascual_main() {
  SHOWIF();
  SHOWREPEATUNTIL();
  SHOWWHILE();
  SHOWFOR();
  SHOWCASE();
}
