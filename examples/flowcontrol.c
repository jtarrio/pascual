/* Program: FLOWCONTROL */
#include "pascual.h"

void SHOWIF() {
  if (1) WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
  else WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
  if (1) WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
  if (1) ;
  else WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
  if (1) ;
  if (1) {
    WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
    WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 8, "true dat");
  }
  else {
    WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
    WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 9, "no really");
  }
  if (1) WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
  else if (1) WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 5, "true2");
  else WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
  if (1) {
    if (1) WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
    else WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
  }
  if (1) if (1) WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
  else WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
}

void SHOWREPEATUNTIL() {
  PInteger I;
  I = 0;
  do {
    I = I + 1;
  } while (!(I == 5));
  I = 0;
  do {
    I = I + 1;
    WRITE(&OUTPUT, RwpInt | RwpLn | RwpEnd, I);
  } while (!(I == 5));
  I = 10;
  do {
    I = I + 1;
  } while (!(I == 5));
  do {
  } while (1);
}

void SHOWWHILE() {
  PInteger I;
  I = 0;
  while (I != 5) I = I + 1;
  I = 0;
  while (I != 5) {
    I = I + 1;
    WRITE(&OUTPUT, RwpInt | RwpLn | RwpEnd, I);
  }
  while (0) ;
}

void SHOWFOR() {
  PInteger I;
  do {
    PInteger first = 1;
    PInteger last = 10;
    if (first <= last) {
      I = first;
      while (1) {
        WRITE(&OUTPUT, RwpInt | RwpLn | RwpEnd, I);
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
  do {
    PInteger first = 10;
    PInteger last = 1;
    if (first >= last) {
      I = first;
      while (1) {
        WRITE(&OUTPUT, RwpInt | RwpLn | RwpEnd, I);
        if (I == last) break;
        --I;
      }
    }
  } while(0);
  do {
    PInteger first = 1;
    PInteger last = 10;
    if (first <= last) {
      I = first;
      while (1) {
        {
          WRITE(&OUTPUT, RwpLenPtr | RwpEnd, 14, "The number is ");
          WRITE(&OUTPUT, RwpInt | RwpLn | RwpEnd, I);
        }
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
  I = 10;
  do {
    PInteger first = 1;
    PInteger last = I;
    if (first <= last) {
      I = first;
      while (1) {
        WRITE(&OUTPUT, RwpInt | RwpLn | RwpEnd, I);
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
  I = 0;
  do {
    PInteger first = 20;
    PInteger last = 10;
    if (first <= last) {
      I = first;
      while (1) {
        WRITE(&OUTPUT, RwpInt | RwpLn | RwpEnd, I);
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
  WRITE(&OUTPUT, RwpLenPtr, 29, "The value of i is 0, not 20: ", RwpInt | RwpLn | RwpEnd, I);
}

void SHOWCASE() {
  PChar I;
  I = 'b';
  switch (I) {
    case 'a':
      WRITE(&OUTPUT, RwpChar | RwpLn | RwpEnd, 'a');
      break;
    case 'b':
      WRITE(&OUTPUT, RwpChar | RwpLn | RwpEnd, 'b');
      break;
    case 'c':
      WRITE(&OUTPUT, RwpChar | RwpLn | RwpEnd, 'c');
      break;
    default:
      break;
  }
  switch (I) {
    case 'd':
      WRITE(&OUTPUT, RwpChar | RwpLn | RwpEnd, 'd');
      break;
    case 'e':
      WRITE(&OUTPUT, RwpChar | RwpLn | RwpEnd, 'e');
      break;
    default:
      WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 5, "other");
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
